#include "plotbuilder.h"

#include <chart/speclayout/bubblechartbuilder.h>
#include <chart/speclayout/tablechart.h>
#include <chart/speclayout/treemap.h>
#include <numeric>

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
		auto &&markerId = index.oldAggr;
		auto needInfo = first != last && first->first == markerId;

		auto &marker = plot->markers.emplace_back(*plot->getOptions(),
		    dataCube,
		    plot->axises,
		    index,
		    plot->markers.size(),
		    needInfo);

		mainBuckets[marker.mainId.seriesId][marker.mainId.itemId] =
		    &marker;
		subBuckets[marker.subId.seriesId][marker.subId.itemId] =
		    &marker;

		while (needInfo) {
			plot->markersInfo.insert({first++->second,
			    Plot::MarkerInfo{Plot::MarkerInfoContent{marker}}});
			needInfo = first != last && first->first == markerId;
		}
	}
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

std::vector<PlotBuilder::BucketInfo>
PlotBuilder::sortedBuckets(const Buckets &buckets, bool main) const
{
	std::vector<BucketInfo> sorted(buckets.inner_size());

	for (auto &&bucket : buckets)
		for (std::size_t ix{}; auto &&marker : bucket) {
			auto &[f, s, has] = sorted[ix];
			f = ix++;
			if (!marker || static_cast<bool>(!marker->enabled))
				continue;
			s += marker->size.getCoord(
			    !plot->getOptions()->isHorizontal());
			has = true;
		}

	if (main && plot->getOptions()->sort == Sort::byValue)
		std::sort(sorted.begin(),
		    sorted.end(),
		    [](const BucketInfo &lhs, const BucketInfo &rhs)
		    {
			    if (auto ord = std::weak_order(lhs.size, rhs.size);
			        !std::is_eq(ord))
				    return std::is_lt(ord);
			    return lhs.index < rhs.index;
		    });

	if (main && plot->getOptions()->reverse)
		std::reverse(sorted.begin(), sorted.end());

	return sorted;
}

void PlotBuilder::addSpecLayout(Buckets &buckets)
{
	auto geometry = plot->getOptions()
	                    ->geometry.get_or_first(::Anim::first)
	                    .value;
	if (auto &markers = plot->markers; isConnecting(geometry)) {
		Charts::TableChart::setupVector(markers, true);
	}
	else if (auto &&size = plot->getOptions()->getChannels().at(
	             ChannelId::size);
	         size.isDimension()) {
		Charts::TableChart::setupVector(markers);
	}
	else if (!dataCube.empty()) {
		buckets.clear();
		buckets.resize(dataCube.combinedSizeOf(size.dimensions()));

		for (auto &marker : markers)
			if (marker.enabled)
				buckets[marker.sizeId.seriesId]
				       [marker.sizeId.itemId] = &marker;

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
	auto &&sorted = sortedBuckets(buckets, main);
	std::erase_if(sorted,
	    std::not_fn(std::mem_fn(&BucketInfo::hasElement)));

	std::vector<double> dimOffset(sorted.size() + 1);

	auto channelId = main ? plot->getOptions()->mainAxisType()
	                      : plot->getOptions()->subAxisType();
	auto subChannelId = main ? plot->getOptions()->subAxisType()
	                         : plot->getOptions()->mainAxisType();
	auto &&axis = plot->getOptions()->getChannels().at(channelId);
	auto &&subAxis =
	    plot->getOptions()->getChannels().at(subChannelId);

	auto horizontal = plot->getOptions()->isHorizontal();
	double Geom::Point::*const coord =
	    horizontal == main ? &Geom::Point::x : &Geom::Point::y;

	auto isAggregatable = axis.isDimension()
	                   || (main && subAxis.isMeasure()
	                       && plot->getOptions()->geometry.get()
	                              == ShapeType::rectangle);

	if (isAggregatable) {
		for (std::size_t ix{}, max = sorted.size(); ix < max; ++ix) {
			auto &o = dimOffset[ix];
			for (const auto &bucket : buckets) {
				auto *marker = bucket[sorted[ix].index];
				if (!marker || static_cast<bool>(!marker->enabled))
					continue;
				o = std::max(o, std::abs(marker->size.*coord));
			}
		}
		std::exclusive_scan(dimOffset.begin(),
		    dimOffset.end(),
		    dimOffset.begin(),
		    0.0);
	}

	bool hasConnection{};

	auto polar =
	    plot->getOptions()->coordSystem.get() == CoordSystem::polar;
	auto connecting =
	    isConnecting(plot->getOptions()->geometry.get());

	auto needPolarConnection =
	    polar && connecting && main && horizontal;

	for (const auto &bucket : buckets) {
		double prevPos{};
		for (auto i = 0U; i < sorted.size(); ++i) {
			auto idAct = sorted[i].index;
			auto *act = bucket[idAct];

			auto iNext = (i + 1) % sorted.size();
			auto idNext = sorted[iNext].index;

			auto *next = bucket[idNext];

			if (act) {
				prevPos = act->position.*coord +=
				    isAggregatable ? dimOffset[i] : prevPos;

				if (isAggregatable && act->size.*coord < 0) {
					act->position.*coord +=
					    dimOffset[i + 1] - dimOffset[i];
				}
			}

			hasConnection |=
			    Marker::connectMarkers(iNext == 0 && act != next,
			        act,
			        next,
			        main,
			        needPolarConnection);
		}
	}
	return hasConnection;
}

void PlotBuilder::normalizeXY()
{
	const auto &xrange =
	    plot->getOptions()->getHorizontalAxis().range;
	const auto &yrange = plot->getOptions()->getVeritalAxis().range;

	auto markerIt = plot->markers.begin();
	while (markerIt != plot->markers.end()
	       && !static_cast<bool>(markerIt->enabled))
		++markerIt;

	if (markerIt == plot->markers.end()) {
		getMeasTrackRange(ChannelId::x) = xrange.getRange({0.0, 0.0});
		getMeasTrackRange(ChannelId::y) = yrange.getRange({0.0, 0.0});
		return;
	}

	auto boundRect = markerIt->toRectangle();

	while (++markerIt != plot->markers.end()) {
		if (!markerIt->enabled) continue;
		boundRect = boundRect.boundary(markerIt->toRectangle());
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
			if (!marker.enabled) continue;

			const auto &id =
			    (type == ChannelId::x)
			            == plot->getOptions()->isHorizontal()
			        ? marker.mainId
			        : marker.subId;

			if (const auto &slice = id.label)
				axis.add(*slice,
				    static_cast<double>(id.itemId),
				    marker.getSizeBy(type == ChannelId::x),
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
			    && axis.add(*sliceIndex, i, {count, count}, merge))
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
			if (marker && static_cast<bool>(marker->enabled))
				range.include(marker->getSizeBy(vectical));

		auto &&transform = align.getAligned(range) / range;

		for (auto &&marker : bucket)
			if (marker)
				marker->setSizeBy(vectical,
				    marker->getSizeBy(vectical) * transform);
	}
}

void PlotBuilder::addSeparation(const Buckets &subBuckets,
    const std::size_t &mainBucketSize) const
{
	if (plot->getOptions()->split) {
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
				if (marker && static_cast<bool>(marker->enabled)) {
					ranges[i].include(
					    marker->getSizeBy(vertical).size());
					anyEnabled[i] = true;
				}
				++i %= ranges.size();
			}

		auto max = Math::Range(0.0, 0.0);
		for (auto i = 0U; i < ranges.size(); ++i)
			if (anyEnabled[i]) max = max + ranges[i];

		for (auto i = 1U; i < ranges.size(); ++i)
			ranges[i] = ranges[i] + ranges[i - 1].getMax()
			          + (anyEnabled[i - 1] ? max.getMax() / 15 : 0);

		for (auto &&bucket : subBuckets)
			for (auto i = 0U; auto &&marker : bucket) {
				if (marker)
					marker->setSizeBy(vertical,
					    Base::Align{align, ranges[i]}.getAligned(
					        marker->getSizeBy(vertical)));
				++i %= ranges.size();
			}
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
		if (!marker.enabled) continue;
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
		if (!marker.enabled) continue;
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