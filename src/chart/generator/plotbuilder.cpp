#include "plotbuilder.h"

#include <chart/speclayout/bubblechartbuilder.h>
#include <chart/speclayout/tablechart.h>
#include <chart/speclayout/treemap.h>

#include "plot.h"

namespace Vizzu::Gen
{

PlotBuilder::PlotBuilder(const Data::DataTable &dataTable,
    const PlotOptionsPtr &options,
    const Styles::Chart &style) :
    dataCube(dataTable, *options),
    plot(std::make_shared<Plot>(options, style))
{
	initDimensionTrackers();

	std::size_t mainBucketSize{};
	auto &&subBuckets = generateMarkers(mainBucketSize);

	if (!plot->options->getChannels().anyAxisSet()) {
		addSpecLayout(subBuckets);
		calcDimensionAxises();
		normalizeColors();
		if (plot->options->geometry != ShapeType::circle)
			normalizeSizes();
		calcMeasureAxises(dataTable);
	}
	else {
		addSeparation(subBuckets, mainBucketSize);
		normalizeXY();
		calcDimensionAxises();
		normalizeSizes();
		normalizeColors();
		calcMeasureAxises(dataTable);
		addAlignment(subBuckets);
	}

	resetDimensionTrackers();
}

void PlotBuilder::initDimensionTrackers() const
{
	for (const Channel &ch :
	    plot->options->getChannels().getChannels()) {
		if (!ch.isDimension()) continue;
		plot->axises.at(ch.type).dimension.trackedValues =
		    std::make_shared<
		        std::vector<std::optional<Data::SliceIndex>>>(
		        dataCube.combinedSizeOf(ch.dimensions()).second);
	}
}

void PlotBuilder::resetDimensionTrackers() const
{
	for (const Channel &ch :
	    plot->options->getChannels().getChannels()) {
		if (!ch.isDimension()) continue;
		plot->axises.at(ch.type).dimension.trackedValues.reset();
	}
}

Buckets PlotBuilder::generateMarkers(std::size_t &mainBucketSize)
{
	Buckets mainBuckets;
	Buckets subBuckets;
	if (!dataCube.empty()) {
		mainBuckets.resize(dataCube.combinedSizeOf(
		    plot->getOptions()->mainAxis().dimensions()));
		mainBucketSize = mainBuckets.size();

		Data::SeriesList subIds(
		    plot->getOptions()->subAxis().dimensions());
		if (plot->getOptions()->geometry == ShapeType::area)
			subIds.split_by(
			    plot->getOptions()->mainAxis().dimensions());
		subBuckets.resize(dataCube.combinedSizeOf(subIds));

		plot->markers.reserve(dataCube.combinedSizeOf({}).first);
	}

	std::multimap<Marker::MarkerIndex, Options::MarkerInfoId> map;
	for (auto &&[ix, mid] : plot->getOptions()->markersInfo)
		map.emplace(mid, ix);

	for (auto first = map.begin(), last = map.end();
	     auto &&index : dataCube) {
		auto &&markerId = plot->markers.size();
		auto needInfo = first != last && first->first == markerId;

		auto &marker = plot->markers.emplace_back(*plot->getOptions(),
		    dataCube,
		    plot->axises,
		    index,
		    markerId,
		    needInfo);

		mainBuckets[marker.mainId.get().seriesId]
		           [marker.mainId.get().itemId] = &marker;
		subBuckets[marker.subId.seriesId][marker.subId.itemId] =
		    &marker;

		while (needInfo) {
			plot->markersInfo.insert({first++->second,
			    Plot::MarkerInfo{Plot::MarkerInfoContent{marker}}});
			needInfo = first != last && first->first == markerId;
		}
	}
	clearEmptyBuckets(mainBuckets, true);
	clearEmptyBuckets(subBuckets, false);
	auto &&hasMarkerConnection = linkMarkers(mainBuckets, true);
	[[maybe_unused]] auto &&_ = linkMarkers(subBuckets, false);

	if (hasMarkerConnection
	    && plot->getOptions()->geometry.get() == ShapeType::line
	    && plot->getOptions()
	           ->getChannels()
	           .at(ChannelId::x)
	           .isDimension()
	    && plot->getOptions()
	           ->getChannels()
	           .at(ChannelId::y)
	           .isDimension()) {
		plot->markerConnectionOrientation.emplace(
		    *plot->getOptions()->orientation.get());
	}
	return subBuckets;
}

std::vector<std::pair<double, std::size_t>>
PlotBuilder::sortedBuckets(const Buckets &buckets, bool main) const
{
	std::vector<std::pair<double, std::size_t>> sorted(
	    buckets.inner_size());

	for (auto &&bucket : buckets)
		for (std::size_t ix{}; auto &&marker : bucket) {
			auto &[s, f] = sorted[ix];
			f = ix++;
			s += marker->size.getCoord(
			    !plot->getOptions()->isHorizontal());
		}

	if (main && plot->getOptions()->sort == Sort::byValue)
		std::sort(sorted.begin(),
		    sorted.end(),
		    [](const std::pair<double, std::size_t> &lhs,
		        const std::pair<double, std::size_t> &rhs)
		    {
			    if (auto ord = std::weak_order(lhs.first, rhs.first);
			        !std::is_eq(ord))
				    return std::is_lt(ord);
			    return lhs.second < rhs.second;
		    });

	if (main && plot->getOptions()->reverse)
		std::reverse(sorted.begin(), sorted.end());

	return sorted;
}

void PlotBuilder::clearEmptyBuckets(const Buckets &buckets,
    bool main) const
{
	for (auto &&bucket : buckets)
		if (!std::any_of(bucket.begin(),
		        bucket.end(),
		        std::mem_fn(&Marker::enabled)))
			for (auto &&marker : bucket)
				marker->resetSize(
				    plot->getOptions()->isHorizontal() == !main);
}

void PlotBuilder::addSpecLayout(Buckets &buckets)
{
	auto geometry = plot->getOptions()
	                    ->geometry.get_or_first(::Anim::first)
	                    .value;
	if (auto &markers = plot->markers;
	    geometry == ShapeType::line || geometry == ShapeType::area) {
		Charts::TableChart::setupVector(markers, true);
	}
	else if (auto &&size = plot->getOptions()->getChannels().at(
	             ChannelId::size);
	         size.isEmpty()) {
		Charts::TableChart::setupVector(markers);
	}
	else if (!dataCube.empty()) {
		buckets.resize(dataCube.combinedSizeOf(size.dimensions()));

		for (auto &marker : markers)
			buckets[marker.sizeId.seriesId][marker.sizeId.itemId] =
			    &marker;

		if (geometry == ShapeType::circle) {
			Charts::BubbleChartBuilder::setupVector(
			    *plot->getStyle().plot.marker.circleMaxRadius,
			    buckets);

			plot->keepAspectRatio = true;
		}
		else
			Charts::TreeMapBuilder::setupVector(buckets);
	}
}

Math::Range<double> &PlotBuilder::getMeasTrackRange(
    ChannelId type) const
{
	return plot->axises.at(type).measure.trackedRange;
}

bool PlotBuilder::linkMarkers(const Buckets &buckets, bool main) const
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
			    plot->getOptions()->isHorizontal() == main,
			    main);
			if (act.enabled && next.enabled && idAct != idNext)
				hasConnection = true;
		}
	return hasConnection;
}

void PlotBuilder::normalizeXY()
{
	const auto &xrange =
	    plot->getOptions()->getHorizontalAxis().range;
	const auto &yrange = plot->getOptions()->getVeritalAxis().range;

	if (plot->markers.empty()) {
		getMeasTrackRange(ChannelId::x) = xrange.getRange({0.0, 0.0});
		getMeasTrackRange(ChannelId::y) = yrange.getRange({0.0, 0.0});
		return;
	}

	auto boundRect = plot->markers.front().toRectangle();

	for (auto &marker : plot->markers) {
		if (!marker.enabled) continue;
		boundRect = boundRect.boundary(marker.toRectangle());
	}

	plot->getOptions()->setAutoRange(
	    boundRect.positive().hSize().getMin() >= 0,
	    boundRect.positive().vSize().getMin() >= 0);

	boundRect.setHSize(xrange.getRange(boundRect.hSize()));
	boundRect.setVSize(yrange.getRange(boundRect.vSize()));

	for (auto &marker : plot->markers) {
		if (!boundRect.intersects(marker.toRectangle().positive()))
			marker.enabled = false;

		auto rect = marker.toRectangle();
		auto newRect = boundRect.normalize(rect);
		marker.fromRectangle(newRect);
	}

	getMeasTrackRange(ChannelId::x) = boundRect.hSize();
	getMeasTrackRange(ChannelId::y) = boundRect.vSize();
}

void PlotBuilder::calcMeasureAxises(const Data::DataTable &dataTable)
{
	for (const Channel &ch :
	    plot->getOptions()->getChannels().getChannels())
		calcMeasureAxis(dataTable, ch.type);
}

void PlotBuilder::calcMeasureAxis(const Data::DataTable &dataTable,
    ChannelId type)
{
	auto &axis = plot->axises.at(type).measure;
	const auto &scale = plot->getOptions()->getChannels().at(type);
	auto range = getMeasTrackRange(type);
	if (auto &&meas = scale.measureId) {
		plot->axises.at(type).common.title =
		    scale.title.isAuto() ? dataCube.getName(*meas)
		    : scale.title        ? *scale.title
		                         : std::string{};

		if (type == plot->getOptions()->subAxisType()
		    && plot->getOptions()->align
		           == Base::Align::Type::stretch) {
			axis = {Math::Range<double>::Raw(0, 100),
			    "%",
			    meas->getColIndex(),
			    scale.step.getValue()};
		}
		else {
			axis = {range.isReal() ? range
			                       : Math::Range<double>::Raw(0, 0),
			    dataTable.getUnit(meas->getColIndex()),
			    meas->getColIndex(),
			    scale.step.getValue()};
		}
	}
	else
		axis = {};
	axis.trackedRange = range;
}

void PlotBuilder::calcDimensionAxises()
{
	for (const Channel &ch :
	    plot->getOptions()->getChannels().getChannels())
		calcDimensionAxis(ch.type);
}

void PlotBuilder::calcDimensionAxis(ChannelId type)
{
	auto &axis = plot->axises.at(type).dimension;
	auto &scale = plot->getOptions()->getChannels().at(type);

	if (scale.isMeasure() || !scale.hasDimension()) return;

	auto &&isTypeAxis = isAxis(type);
	if (auto merge = scale.labelLevel == 0; isTypeAxis) {
		for (const auto &marker : plot->markers) {
			const auto &id =
			    (type == ChannelId::x)
			            == plot->getOptions()->isHorizontal()
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
		const auto &indices =
		    *plot->axises.at(type).dimension.trackedValues;

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

	plot->axises.at(type).common.title =
	    scale.title.isAuto() && !hasLabel ? axis.category
	    : scale.title                     ? *scale.title
	                                      : std::string{};
}

void PlotBuilder::addAlignment(const Buckets &subBuckets) const
{
	if (static_cast<bool>(plot->getOptions()->split)) return;

	if (plot->axises.at(plot->getOptions()->subAxisType())
	        .measure.range.getMin()
	    < 0)
		return;

	if (plot->getOptions()->align == Base::Align::Type::none) return;

	auto &&vectical = !plot->getOptions()->isHorizontal();
	const Base::Align align{plot->getOptions()->align,
	    Math::Range(0.0, 1.0)};
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

void PlotBuilder::addSeparation(const Buckets &subBuckets,
    const std::size_t &mainBucketSize) const
{
	if (static_cast<bool>(plot->getOptions()->split)) {
		auto align =
		    plot->getOptions()->align == Base::Align::Type::none
		        ? Base::Align::Type::min
		        : plot->getOptions()->align;

		std::vector ranges{mainBucketSize,
		    Math::Range<double>::Raw({}, {})};
		std::vector<bool> anyEnabled(mainBucketSize);

		auto &&vertical = !plot->getOptions()->isHorizontal();
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

void PlotBuilder::normalizeSizes()
{
	if (plot->getOptions()->geometry == ShapeType::circle
	    || plot->getOptions()->geometry == ShapeType::line) {
		Math::Range<double> size;

		for (auto &marker : plot->markers)
			if (marker.enabled) size.include(marker.sizeFactor);

		size = plot->getOptions()
		           ->getChannels()
		           .at(ChannelId::size)
		           .range.getRange(size);

		for (auto &marker : plot->markers)
			marker.sizeFactor = size.getMax() == size.getMin()
			                      ? 0
			                      : size.normalize(marker.sizeFactor);
	}
	else
		for (auto &marker : plot->markers) marker.sizeFactor = 0;
}

void PlotBuilder::normalizeColors()
{
	Math::Range<double> lightness;
	Math::Range<double> color;

	for (auto &marker : plot->markers) {
		auto &&cbase = marker.colorBase.get();
		if (!cbase.isDiscrete()) color.include(cbase.getPos());
		lightness.include(cbase.getLightness());
	}

	color = plot->getOptions()
	            ->getChannels()
	            .at(ChannelId::color)
	            .range.getRange(color);
	lightness = plot->getOptions()
	                ->getChannels()
	                .at(ChannelId::lightness)
	                .range.getRange(lightness);

	for (auto &marker : plot->markers) {
		auto &&cbase = marker.colorBase->value;
		cbase.setLightness(lightness.rescale(cbase.getLightness()));

		if (!cbase.isDiscrete())
			cbase.setPos(color.rescale(cbase.getPos()));
	}

	getMeasTrackRange(ChannelId::color) = color;
	getMeasTrackRange(ChannelId::lightness) = lightness;

	for (auto &value : plot->axises.at(ChannelId::color).dimension)
		value.second.colorBase =
		    ColorBase(static_cast<uint32_t>(value.second.value), 0.5);

	for (auto &value :
	    plot->axises.at(ChannelId::lightness).dimension) {
		value.second.value = lightness.rescale(value.second.value);
		value.second.colorBase = ColorBase(0U, value.second.value);
	}
}

}