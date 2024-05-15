#include "plot.h"

#include <limits>
#include <numeric>

#include "base/anim/interpolated.h"
#include "base/conv/numtostr.h"
#include "base/math/range.h"
#include "chart/speclayout/bubblechartbuilder.h"
#include "chart/speclayout/tablechart.h"
#include "chart/speclayout/treemap.h"

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
    info{marker.cellInfo, &marker.cellInfo->markerInfo}
{}

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
    markers(other.markers),
    markersInfo(other.markersInfo)
{}

Plot::Plot(const Data::DataTable &dataTable,
    PlotOptionsPtr opts,
    Styles::Chart style) :
    dataTable(dataTable),
    options(std::move(opts)),
    style(std::move(style))
{
	Data::DataCube dataCube(dataTable, *options);
	this->dataCube = &dataCube;
	ChannelsStats stats(options->getChannels(), getDataCube());
	this->stats = &stats;

	anyAxisSet = options->getChannels().anyAxisSet();

	auto &&subBuckets = generateMarkers();

	if (!options->getChannels().anyAxisSet()) {
		addSpecLayout(subBuckets);
		calcDimensionAxises();
		normalizeColors();
		if (options->geometry != ShapeType::circle) normalizeSizes();
		calcMeasureAxises();
	}
	else {
		addSeparation(subBuckets);
		normalizeXY();
		calcDimensionAxises();
		normalizeSizes();
		normalizeColors();
		calcMeasureAxises();
		addAlignment(subBuckets);
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

Buckets Plot::generateMarkers()
{
	Buckets mainBuckets;
	Buckets subBuckets;
	if (!getDataCube().empty()) {
		mainBuckets.resize(getDataCube().combinedSizeOf(
		    options->mainAxis().dimensions()));
		mainBucketSize = mainBuckets.size();

		Data::SeriesList subIds(options->subAxis().dimensions());
		if (getOptions()->geometry == ShapeType::area)
			subIds.split_by(options->mainAxis().dimensions());
		subBuckets.resize(getDataCube().combinedSizeOf(subIds));

		markers.reserve(getDataCube().combinedSizeOf({}).first);
	}

	std::multimap<Marker::MarkerIndex, Options::MarkerInfoId> map;
	for (auto &&[ix, mid] : options->markersInfo)
		map.emplace(mid, ix);

	bool rectangleSpacing{true};
	if (auto &&sp = style.plot.marker.rectangleSpacing;
	    sp && sp->get().has_value())
		rectangleSpacing = *sp->get() > 0.0;

	for (auto first = map.begin(), last = map.end();
	     auto &&index : getDataCube()) {
		auto &&markerId = markers.size();
		auto needInfo = first != last && first->first == markerId;

		auto &marker = markers.emplace_back(*options,
		    getDataCube(),
		    getStats(),
		    index,
		    markerId,
		    needInfo,
		    rectangleSpacing);

		mainBuckets[marker.mainId.get().seriesId]
		           [marker.mainId.get().itemId] = &marker;
		subBuckets[marker.subId.seriesId][marker.subId.itemId] =
		    &marker;

		while (needInfo) {
			markersInfo.insert({first++->second,
			    MarkerInfo{MarkerInfoContent{marker}}});
			needInfo = first != last && first->first == markerId;
		}
	}
	clearEmptyBuckets(mainBuckets, true);
	clearEmptyBuckets(subBuckets, false);
	auto &&hasMarkerConnection = linkMarkers(mainBuckets, true);
	[[maybe_unused]] auto &&_ = linkMarkers(subBuckets, false);

	if (hasMarkerConnection
	    && options->geometry.get() == ShapeType::line
	    && options->getChannels().at(ChannelId::x).isDimension()
	    && options->getChannels().at(ChannelId::y).isDimension()) {
		markerConnectionOrientation.emplace(
		    *options->orientation.get());
	}
	return subBuckets;
}

std::vector<std::pair<double, std::size_t>>
Plot::sortedBuckets(const Buckets &buckets, bool main) const
{
	std::vector<std::pair<double, std::size_t>> sorted(
	    buckets.inner_size());

	for (auto &&bucket : buckets)
		for (std::size_t ix{}; auto &&marker : bucket) {
			auto &[s, f] = sorted[ix];
			f = ix++;
			s += marker->size.getCoord(!options->isHorizontal());
		}

	if (main && options->sort == Sort::byValue)
		std::sort(sorted.begin(), sorted.end());

	if (main && options->reverse)
		std::reverse(sorted.begin(), sorted.end());

	return sorted;
}

void Plot::clearEmptyBuckets(const Buckets &buckets, bool main) const
{
	for (auto &&bucket : buckets)
		if (!std::any_of(bucket.begin(),
		        bucket.end(),
		        std::mem_fn(&Marker::enabled)))
			for (auto &&marker : bucket)
				marker->resetSize(options->isHorizontal() == !main);
}

void Plot::addSpecLayout(Buckets &buckets)
{
	auto geometry = getOptions()->geometry.get(::Anim::first).value;
	if (auto &markers = getMarkers();
	    geometry == ShapeType::line || geometry == ShapeType::area) {
		Charts::TableChart::setupVector(markers, true);
	}
	else if (auto &&size =
	             getOptions()->getChannels().at(ChannelId::size);
	         size.isEmpty()) {
		Charts::TableChart::setupVector(markers);
	}
	else if (!getDataCube().empty()) {
		buckets.resize(
		    getDataCube().combinedSizeOf(size.dimensions()));

		for (auto &marker : markers)
			buckets[marker.sizeId.seriesId][marker.sizeId.itemId] =
			    &marker;

		if (geometry == ShapeType::circle) {
			Charts::BubbleChartBuilder::setupVector(
			    *getStyle().plot.marker.circleMaxRadius,
			    buckets);

			keepAspectRatio = true;
		}
		else
			Charts::TreeMap::setupVector(buckets);
	}
}

bool Plot::linkMarkers(const Buckets &buckets, bool main) const
{
	bool hasConnection{};
	for (auto &&sorted = sortedBuckets(buckets, main);
	     const auto &bucket : buckets)
		for (auto i = 0U; i < sorted.size(); ++i) {
			auto idAct = sorted[i].second;
			auto &act = *bucket[idAct];
			auto iNext = (i + 1) % sorted.size();
			auto idNext = sorted[iNext].second;
			auto &next = *bucket[idNext];
			act.setNextMarker(iNext == 0,
			    next,
			    options->isHorizontal() == main,
			    main);
			if (act.enabled && next.enabled && idAct != idNext)
				hasConnection = true;
		}
	return hasConnection;
}

void Plot::normalizeXY()
{
	const auto &xrange = options->getHorizontalAxis().range;
	const auto &yrange = options->getVeritalAxis().range;

	if (markers.empty()) {
		getStats().channels[ChannelId::x].range() =
		    xrange.getRange({0.0, 0.0});
		getStats().channels[ChannelId::y].range() =
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

	getStats().channels[ChannelId::x].range() = boundRect.hSize();
	getStats().channels[ChannelId::y].range() = boundRect.vSize();
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
	if (auto &&meas = scale.measureId) {
		commonAxises.at(type).title = scale.title.isAuto()
		                                ? dataCube->getName(*meas)
		                            : scale.title ? *scale.title
		                                          : std::string{};

		if (type == options->subAxisType()
		    && options->align == Base::Align::Type::stretch) {
			axis = {Math::Range<double>::Raw(0, 100),
			    "%",
			    meas->getColIndex(),
			    scale.step.getValue()};
		}
		else {
			auto range = getStats().channels[type].range();
			if (!range.isReal())
				range = Math::Range<double>::Raw({}, {});

			axis = {range,
			    dataTable.getUnit(meas->getColIndex()),
			    meas->getColIndex(),
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

	if (scale.isMeasure() || !scale.hasDimension()) return;

	auto &&isTypeAxis = isAxis(type);
	if (auto merge = scale.labelLevel == 0; isTypeAxis) {
		for (const auto &marker : markers) {
			const auto &id =
			    (type == ChannelId::x) == options->isHorizontal()
			        ? marker.mainId.get()
			        : marker.subId;

			if (const auto &slice = id.label)
				axis.add(*slice,
				    static_cast<double>(id.itemId),
				    marker.getSizeBy(type == ChannelId::x),
				    static_cast<double>(marker.enabled),
				    merge);
		}
	}
	else {
		const auto &indices = getStats().channels[type].indices();

		double count = 0;
		for (auto i = 0U; i < indices.size(); ++i)
			if (const auto &sliceIndex = indices[i];
			    sliceIndex
			    && axis.add(*sliceIndex,
			        i,
			        {count, count},
			        true,
			        merge))
				count += 1;
	}
	auto hasLabel =
	    axis.setLabels(isTypeAxis ? scale.step.getValue(1.0) : 1.0);

	if (auto &&series = scale.labelSeries())
		axis.category = series.value().getColIndex();

	commonAxises.at(type).title = scale.title.isAuto() && !hasLabel
	                                ? axis.category
	                            : scale.title ? *scale.title
	                                          : std::string{};
}

void Plot::addAlignment(const Buckets &subBuckets) const
{
	if (static_cast<bool>(options->split)) return;

	if (measureAxises.at(options->subAxisType()).range.getMin() < 0)
		return;

	if (options->align == Base::Align::Type::none) return;

	auto &&vectical = !options->isHorizontal();
	const Base::Align align{options->align, Math::Range(0.0, 1.0)};
	for (auto &&bucket : subBuckets) {
		Math::Range<double> range;

		for (auto &&marker : bucket)
			range.include(marker->getSizeBy(vectical));

		auto &&transform = align.getAligned(range) / range;

		for (auto &&marker : bucket)
			marker->setSizeBy(vectical,
			    marker->getSizeBy(vectical) * transform);
	}
}

void Plot::addSeparation(const Buckets &subBuckets) const
{
	if (static_cast<bool>(options->split)) {
		auto align = options->align == Base::Align::Type::none
		               ? Base::Align::Type::min
		               : options->align;

		std::vector ranges{mainBucketSize,
		    Math::Range<double>::Raw({}, {})};
		std::vector<bool> anyEnabled(mainBucketSize);

		auto &&vertical = !options->isHorizontal();
		for (auto &&bucket : subBuckets)
			for (auto i = 0U; auto &&marker : bucket) {
				ranges[i].include(marker->getSizeBy(vertical).size());
				if (static_cast<double>(marker->enabled) > 0)
					anyEnabled[i] = true;
				++i %= ranges.size();
			}

		auto max = Math::Range(0.0, 0.0);
		for (auto i = 1U; i < ranges.size(); ++i)
			if (anyEnabled[i]) max = max + ranges[i];

		for (auto i = 1U; i < ranges.size(); ++i)
			ranges[i] = ranges[i] + ranges[i - 1].getMax()
			          + (anyEnabled[i - 1] ? max.getMax() / 15 : 0);

		for (auto &&bucket : subBuckets)
			for (auto i = 0U; auto &&marker : bucket)
				marker->setSizeBy(vertical,
				    Base::Align{align, ranges[(i %= ranges.size())++]}
				        .getAligned(marker->getSizeBy(vertical)));
	}
}

void Plot::normalizeSizes()
{
	if (options->geometry == ShapeType::circle
	    || options->geometry == ShapeType::line) {
		Math::Range<double> size;

		for (auto &marker : markers)
			if (marker.enabled) size.include(marker.sizeFactor);

		size = options->getChannels()
		           .at(ChannelId::size)
		           .range.getRange(size);

		for (auto &marker : markers)
			marker.sizeFactor = size.getMax() == size.getMin()
			                      ? 0
			                      : size.normalize(marker.sizeFactor);
	}
	else
		for (auto &marker : markers) marker.sizeFactor = 0;
}

void Plot::normalizeColors()
{
	Math::Range<double> lightness;
	Math::Range<double> color;

	for (auto &marker : markers) {
		auto &&cbase = marker.colorBase.get();
		if (!cbase.isDiscrete()) color.include(cbase.getPos());
		lightness.include(cbase.getLightness());
	}

	color = options->getChannels()
	            .at(ChannelId::color)
	            .range.getRange(color);
	lightness = options->getChannels()
	                .at(ChannelId::lightness)
	                .range.getRange(lightness);

	for (auto &marker : markers) {
		auto &&cbase = marker.colorBase->value;
		cbase.setLightness(lightness.rescale(cbase.getLightness()));

		if (!cbase.isDiscrete())
			cbase.setPos(color.rescale(cbase.getPos()));
	}

	getStats().channels[ChannelId::color].range() = color;
	getStats().channels[ChannelId::lightness].range() = lightness;

	for (auto &value : dimensionAxises.at(ChannelId::color))
		value.second.colorBase =
		    ColorBase(static_cast<uint32_t>(value.second.value), 0.5);

	for (auto &value : dimensionAxises.at(ChannelId::lightness)) {
		value.second.value = lightness.rescale(value.second.value);
		value.second.colorBase = ColorBase(0U, value.second.value);
	}
}

void Plot::prependMarkers(const Plot &plot)
{
	auto size = plot.markers.size();

	auto it = markers.insert(markers.begin(),
	              plot.getMarkers().begin(),
	              plot.getMarkers().end())
	        + static_cast<std::ptrdiff_t>(size);

	for (auto i = markers.begin(); i < it; ++i) i->enabled = false;

	while (it != markers.end()) it++->setIdOffset(size);
}

void Plot::appendMarkers(const Plot &plot)
{
	auto size = markers.size();

	for (auto it = markers.insert(markers.end(),
	         plot.getMarkers().begin(),
	         plot.getMarkers().end());
	     it != markers.end();
	     ++it) {
		auto &marker = *it;

		marker.setIdOffset(size);
		marker.enabled = false;
	}
}

bool Plot::dimensionMatch(const Plot &a, const Plot &b)
{
	return a.getOptions()->getChannels().getDimensions()
	    == b.getOptions()->getChannels().getDimensions();
}

}
