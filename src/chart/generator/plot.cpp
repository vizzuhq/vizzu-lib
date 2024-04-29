#include "plot.h"

#include <limits>
#include <numeric>

#include "base/anim/interpolated.h"
#include "base/conv/numtostr.h"
#include "base/math/range.h"
#include "chart/speclayout/speclayout.h"

namespace Vizzu::Gen
{

Plot::MarkersInfo interpolate(const Plot::MarkersInfo &op1,
    const Plot::MarkersInfo &op2,
    double factor)
{
	Plot::MarkersInfo result;
	auto iter1 = op1.begin();
	auto iter2 = op2.begin();
	for (; iter1 != op1.end() && iter2 != op2.end();
	     ++iter1, ++iter2) {
		if (iter1->first != iter2->first)
			throw std::logic_error("invalid map operation");
		if (iter1->second.get() || iter2->second.get()) {
			result.insert(std::make_pair(iter1->first,
			    interpolate(iter1->second, iter2->second, factor)));
		}
	}
	return result;
}

Plot::MarkerInfoContent::MarkerInfoContent(const Marker &marker) :
    markerId(marker.idx),
    info(marker.cellInfo.categories)
{
	thread_local auto conv =
	    Conv::NumberToString{.fractionDigitCount = 3};
	for (auto &&[ser, val] : marker.cellInfo.values)
		info.emplace_back(ser, conv(val));
}

Plot::MarkerInfoContent::operator bool() const
{
	return markerId.has_value();
}

bool Plot::MarkerInfoContent::operator==(
    const MarkerInfoContent &op) const
{
	return markerId == op.markerId;
}

Plot::Plot(PlotOptionsPtr options, const Plot &other) :
    anyAxisSet(other.anyAxisSet),
    commonAxises(other.commonAxises),
    measureAxises(other.measureAxises),
    guides(other.guides),
    dimensionAxises(other.dimensionAxises),
    keepAspectRatio(other.keepAspectRatio),
    dataTable(other.dataTable),
    options(std::move(options)),
    style(other.style),
    markersInfo(other.markersInfo)
{}

Plot::Plot(const Data::DataTable &dataTable,
    PlotOptionsPtr opts,
    Styles::Chart style) :
    dataTable(dataTable),
    options(std::move(opts)),
    style(std::move(style)),
    dataCube(std::in_place, dataTable, *options),
    stats(options->getChannels(), getDataCube())
{
	anyAxisSet = options->getChannels().anyAxisSet();

	generateMarkers();
	generateMarkersInfo();

	SpecLayout specLayout(*this);
	auto gotSpecLayout = specLayout.addIfNeeded();

	if (gotSpecLayout) {
		calcDimensionAxises();
		normalizeColors();
		if (options->geometry != ShapeType::circle) normalizeSizes();
		calcMeasureAxises();
	}
	else {
		addSeparation();
		normalizeXY();
		calcDimensionAxises();
		normalizeSizes();
		normalizeColors();
		calcMeasureAxises();
		addAlignment();
	}

	guides.init(*options);
}

void Plot::detachOptions()
{
	options = std::make_shared<Gen::Options>(*options);
}

bool Plot::isEmpty() const
{
	return options->getChannels().isEmpty();
}

void Plot::generateMarkers()
{
	if (!getDataCube().empty()) {
		auto &&[k, v] = getDataCube().combinedSizeOf(
		    options->mainAxis().dimensionIds);
		mainBuckets.resize(k, std::vector<Marker *>(v));

		Data::SeriesList subIds(options->subAxis().dimensionIds);
		if (getOptions()->geometry == ShapeType::area)
		    [[maybe_unused]]
			auto &&_ =
			    subIds.split_by(options->mainAxis().dimensionIds);
		auto &&[k2, v2] = getDataCube().combinedSizeOf(subIds);
		subBuckets.resize(k2, std::vector<Marker *>(v2));

		markers.reserve(getDataCube().combinedSizeOf({}).first);
	}

	for (auto &&index : getDataCube()) {
		auto &marker = markers.emplace_back(*options,
		    getDataCube(),
		    stats,
		    index,
		    markers.size());

		mainBuckets[marker.mainId.get().seriesId]
		           [marker.mainId.get().itemId] = &marker;
		subBuckets[marker.subId.seriesId][marker.subId.itemId] =
		    &marker;
	}
	clearEmptyBuckets(mainBuckets, true);
	clearEmptyBuckets(subBuckets, false);
	auto hasMarkerConnection = linkMarkers(mainBuckets, true);
	linkMarkers(subBuckets, false);

	if (hasMarkerConnection
	    && options->geometry.get() == ShapeType::line
	    && options->getChannels().at(ChannelId::x).isDimension()
	    && options->getChannels().at(ChannelId::y).isDimension()) {
		markerConnectionOrientation.emplace(
		    *options->orientation.get());
	}
}

void Plot::generateMarkersInfo()
{
	for (auto &[ix, mid] : options->markersInfo)
		markersInfo.insert(
		    {ix, MarkerInfo{MarkerInfoContent{markers[mid]}}});
}

std::vector<std::pair<uint64_t, double>>
Plot::sortedBuckets(const Buckets &buckets, bool main)
{
	std::vector<std::pair<uint64_t, double>> sorted;
	if (!buckets.empty()) sorted.resize(buckets.front().size());

	for (const auto &bucket : buckets)
		for (std::size_t ix{}; const auto &marker : bucket) {
			auto &[f, s] = sorted[ix];
			f = ix++;
			s += marker->size.getCoord(!options->isHorizontal());
		}

	if (main && options->sort == Sort::byValue)
		std::sort(sorted.begin(),
		    sorted.end(),
		    [](const std::pair<uint64_t, double> &a,
		        const std::pair<uint64_t, double> &b)
		    {
			    return a.second < b.second;
		    });

	if (main && options->reverse)
		std::reverse(sorted.begin(), sorted.end());

	return sorted;
}

void Plot::clearEmptyBuckets(const Buckets &buckets, bool main)
{
	for (const auto &bucket : buckets) {
		bool enabled = false;

		for (const auto &marker : bucket)
			enabled |= static_cast<bool>(marker->enabled);

		if (!enabled)
			for (const auto &marker : bucket)
				marker->resetSize(options->isHorizontal() == !main);
	}
}

bool Plot::linkMarkers(const Buckets &buckets, bool main)
{
	auto sorted = sortedBuckets(buckets, main);

	bool hasConnection{};
	for (const auto &bucket : buckets) {

		for (auto i = 0U; i < sorted.size(); ++i) {
			auto idAct = sorted[i].first;
			auto indexAct = bucket[idAct];
			auto &act = *indexAct;
			auto iNext = (i + 1) % sorted.size();
			auto idNext = sorted[iNext].first;
			auto indexNext = bucket[idNext];
			auto &next = *indexNext;
			act.setNextMarker(iNext,
			    next,
			    options->isHorizontal() == main,
			    main);
			if (act.enabled && next.enabled && indexAct != indexNext)
				hasConnection = true;
		}
	}
	return hasConnection;
}

void Plot::normalizeXY()
{
	const auto &xrange = options->getHorizontalAxis().range;
	const auto &yrange = options->getVeritalAxis().range;

	if (markers.empty()) {
		stats.channels[ChannelId::x].range =
		    xrange.getRange({0.0, 0.0});
		stats.channels[ChannelId::y].range =
		    yrange.getRange({0.0, 0.0});
		return;
	}

	auto boundRect = markers.front().toRectangle();

	for (auto &marker : markers) {
		if (!marker.enabled) continue;
		boundRect = boundRect.boundary(marker.toRectangle());
	}

	options->setAutoRange(boundRect.positive().hSize().getMin() >= 0,
	    boundRect.positive().vSize().getMin() >= 0);

	boundRect.setHSize(xrange.getRange(boundRect.hSize()));
	boundRect.setVSize(yrange.getRange(boundRect.vSize()));

	for (auto &marker : markers) {
		if (!boundRect.intersects(marker.toRectangle().positive()))
			marker.enabled = false;

		auto rect = marker.toRectangle();
		auto newRect = boundRect.normalize(rect);
		marker.fromRectangle(newRect);
	}

	stats.channels[ChannelId::x].range = boundRect.hSize();
	stats.channels[ChannelId::y].range = boundRect.vSize();
}

void Plot::calcMeasureAxises()
{
	for (auto i = 0U; i < std::size(measureAxises.axises); ++i)
		calcMeasureAxis(static_cast<ChannelId>(i));
}

void Plot::calcMeasureAxis(ChannelId type)
{
	auto &axis = measureAxises.at(type);
	const auto &scale = options->getChannels().at(type);
	if (!scale.isEmpty() && scale.measureId) {
		auto &&name = scale.measureName(*dataCube);
		commonAxises.at(type).title = scale.title.isAuto() ? name
		                            : scale.title ? *scale.title
		                                          : std::string{};

		if (type == options->subAxisType()
		    && options->align == Base::Align::Type::stretch) {
			axis = {Math::Range<double>(0, 100),
			    "%",
			    scale.step.getValue()};
		}
		else {
			auto range = stats.channels[type].range;
			if (!range.isReal())
				range = Math::Range<double>::Raw(0.0, 0.0);

			axis = {range,
			    std::string{dataCube->getUnit(name)},
			    scale.step.getValue()};
		}
	}
	else
		axis = {};
}

void Plot::calcDimensionAxises()
{
	for (auto i = 0U; i < std::size(dimensionAxises.axises); ++i)
		calcDimensionAxis(static_cast<ChannelId>(i));
}

void Plot::calcDimensionAxis(ChannelId type)
{
	auto &axis = dimensionAxises.at(type);
	auto &scale = options->getChannels().at(type);

	if (scale.dimensionIds.empty() || !scale.isDimension()) return;

	auto dim = scale.labelLevel;

	auto &&isTypeAxis = isAxis(type);
	if (isTypeAxis) {
		for (const auto &marker : markers) {
			const auto &id =
			    (type == ChannelId::x) == options->isHorizontal()
			        ? marker.mainId.get()
			        : marker.subId;

			if (const auto &slice = id.itemSliceIndex;
			    dim < slice.size())
				axis.add(slice[dim],
				    static_cast<double>(id.itemId),
				    marker.getSizeBy(type == ChannelId::x),
				    static_cast<double>(marker.enabled),
				    dim == 0);
		}
	}
	else {
		const auto &indices = stats.channels[type].usedIndices;

		double count = 0;
		for (auto i = 0U; i < indices.size(); ++i)
			if (const auto &sliceIndex = indices[i];
			    dim < sliceIndex.size()
			    && axis.add(sliceIndex[dim],
			        i,
			        {count, count},
			        true,
			        dim == 0))
				count += 1;
	}
	auto hasLabel =
	    axis.setLabels(isTypeAxis ? scale.step.getValue(1.0) : 1.0);

	commonAxises.at(type).title = scale.title.isAuto() && !hasLabel
	                                ? scale.labelDimensionName()
	                            : scale.title ? *scale.title
	                                          : std::string{};

	if (auto &&series = scale.labelSeries())
		axis.category = series.value().getColIndex();
}

void Plot::addAlignment()
{
	if (static_cast<bool>(options->split)) return;

	auto &axis = measureAxises.at(options->subAxisType());
	if (axis.range.getMin() < 0) return;

	if (options->align == Base::Align::Type::none) return;

	auto &&vectical = !options->isHorizontal();
	for (auto &bucket : subBuckets) {
		Math::Range<double> range;

		for (auto &marker : bucket)
			range.include(marker->getSizeBy(vectical));

		auto &&transform =
		    Base::Align{options->align, Math::Range(0.0, 1.0)}
		        .getAligned(range)
		    / range;

		for (auto &marker : bucket)
			marker->setSizeBy(vectical,
			    marker->getSizeBy(vectical) * transform);
	}
}

void Plot::addSeparation()
{
	if (static_cast<bool>(options->split)) {
		auto align = options->align == Base::Align::Type::none
		               ? Base::Align::Type::min
		               : options->align;

		std::vector<Math::Range<double>> ranges(mainBuckets.size(),
		    Math::Range(0.0, 0.0));
		std::vector<bool> anyEnabled(mainBuckets.size(), false);

		auto &&vertical = !options->isHorizontal();
		for (auto &bucket : subBuckets) {
			auto i = 0U;
			for (auto &marker : bucket) {
				ranges[i].include(marker->getSizeBy(vertical).size());
				if (static_cast<double>(marker->enabled) > 0)
					anyEnabled[i] = true;
				++i %= ranges.size();
			}
		}

		auto max = Math::Range(0.0, 0.0);
		for (auto i = 1U; i < ranges.size(); ++i)
			if (anyEnabled[i]) max = max + ranges[i];

		for (auto i = 1U; i < ranges.size(); ++i)
			ranges[i] = ranges[i] + ranges[i - 1].getMax()
			          + (anyEnabled[i - 1] ? max.getMax() / 15 : 0);

		for (auto &bucket : subBuckets) {
			auto i = 0U;
			for (auto &marker : bucket)
				marker->setSizeBy(vertical,
				    Base::Align{align, ranges[(i %= ranges.size())++]}
				        .getAligned(marker->getSizeBy(vertical)));
		}
	}
}

void Plot::normalizeSizes()
{
	if (options->geometry == ShapeType::circle
	    || options->geometry == ShapeType::line) {
		Math::Range<double> size;

		for (auto &marker : markers)
			if (marker.enabled) size.include(marker.sizeFactor);

		auto sizeRange =
		    options->getChannels().at(ChannelId::size).range;
		size = sizeRange.getRange(size);

		for (auto &marker : markers)
			marker.sizeFactor = size.getMax() == size.getMin()
			                      ? 0
			                      : size.normalize(marker.sizeFactor);
	}
	else {
		for (auto &marker : markers) marker.sizeFactor = 0;
	}
}

void Plot::normalizeColors()
{
	Math::Range<double> lightness;
	Math::Range<double> color;

	for (auto &marker : markers) {
		if (!marker.colorBase.get().isDiscrete())
			color.include(marker.colorBase.get().getPos());
		lightness.include(marker.colorBase.get().getLightness());
	}

	auto colorRange =
	    options->getChannels().at(ChannelId::color).range;
	color = colorRange.getRange(color);

	auto lightnessRange =
	    options->getChannels().at(ChannelId::lightness).range;
	lightness = lightnessRange.getRange(lightness);

	for (auto &marker : markers) {
		(*marker.colorBase)
		    .value.setLightness(lightness.rescale(
		        marker.colorBase.get().getLightness()));

		if (!marker.colorBase.get().isDiscrete())
			(*marker.colorBase)
			    .value.setPos(
			        color.rescale(marker.colorBase.get().getPos()));
	}

	stats.channels[ChannelId::color].range = color;
	stats.channels[ChannelId::lightness].range = lightness;

	for (auto &value : dimensionAxises.at(ChannelId::color)) {
		value.second.colorBase =
		    ColorBase(static_cast<uint32_t>(value.second.value), 0.5);
	}

	for (auto &value : dimensionAxises.at(ChannelId::lightness)) {
		value.second.value = lightness.rescale(value.second.value);
		value.second.colorBase = ColorBase(0U, value.second.value);
	}
}

void Plot::prependMarkers(const Plot &plot, bool enabled)
{
	auto size = plot.markers.size();

	markers.insert(markers.begin(),
	    plot.getMarkers().begin(),
	    plot.getMarkers().end());

	if (!enabled)
		for (auto i = 0U; i < size; ++i) markers[i].enabled = false;

	for (auto i = size; i < markers.size(); ++i)
		markers[i].setIdOffset(size);
}

void Plot::appendMarkers(const Plot &plot, bool enabled)
{
	auto size = markers.size();

	markers.insert(markers.end(),
	    plot.getMarkers().begin(),
	    plot.getMarkers().end());

	for (auto i = size; i < markers.size(); ++i) {
		auto &marker = markers[i];

		marker.setIdOffset(size);

		if (!enabled) marker.enabled = false;
	}
}

bool Plot::dimensionMatch(const Plot &a, const Plot &b)
{
	return a.getOptions()->getChannels().getDimensions()
	    == b.getOptions()->getChannels().getDimensions();
}

}
