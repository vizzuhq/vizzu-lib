#include "plotbuilder.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

#include "base/anim/interpolated.h"
#include "base/math/floating.h"
#include "base/math/range.h"
#include "base/refl/auto_enum.h"
#include "chart/main/style.h"
#include "chart/options/align.h"
#include "chart/options/channel.h"
#include "chart/options/coordsystem.h"
#include "chart/options/options.h"
#include "chart/options/shapetype.h"
#include "chart/options/sort.h"
#include "chart/speclayout/bubblechartbuilder.h"
#include "chart/speclayout/tablechart.h"
#include "chart/speclayout/treemap.h"
#include "dataframe/old/datatable.h"
#include "dataframe/old/types.h"

#include "buckets.h"
#include "colorbase.h"
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

	if (!plot->getOptions()->getChannels().anyAxisSet()) {
		addSpecLayout(subBuckets);
		normalizeSizes();
	}
	else {
		normalizeSizes();
		addAxisLayout(subBuckets, mainBucketSize, dataTable);
	}

	normalizeColors();
	calcLegendAndLabel(dataTable);
}

void PlotBuilder::addAxisLayout(Buckets &subBuckets,
    const std::size_t &mainBucketSize,
    const Data::DataTable &dataTable)
{
	linkMarkers(subBuckets);
	addSeparation(subBuckets, mainBucketSize);
	calcAxises(dataTable);
	addAlignment(subBuckets);
}

void PlotBuilder::initDimensionTrackers()
{
	for (auto type : Refl::enum_values<ChannelId>())
		if (auto &&ch = plot->getOptions()->getChannels().at(type);
		    !ch.hasMeasure())
			stats.tracked.at(type).emplace<1>(
			    dataCube.combinedSizeOf(ch.dimensions()).second);
}

Buckets PlotBuilder::generateMarkers(std::size_t &mainBucketSize)
{
	const auto &mainIds(plot->getOptions()->mainAxis().dimensions());
	auto subIds(plot->getOptions()->subAxis().dimensions());
	if (!dataCube.empty()) {
		if (plot->getOptions()->geometry == ShapeType::area)
			subIds.split_by(mainIds);

		mainBucketSize = dataCube.combinedSizeOf(mainIds).first;
		plot->markers.reserve(dataCube.df->get_record_count());
	}

	using CmpBySec = decltype(
		[] (const std::pair<const Options::MarkerInfoId, Marker::MarkerIndex>& lhs,
			const std::pair<const Options::MarkerInfoId, Marker::MarkerIndex>& rhs)
		{
			return lhs.second < rhs.second;
		});

	std::multiset<std::reference_wrapper<
	                  const std::pair<const Options::MarkerInfoId,
	                      Marker::MarkerIndex>>,
	    CmpBySec>
	    set{plot->getOptions()->markersInfo.begin(),
	        plot->getOptions()->markersInfo.end()};

	for (auto first = set.begin(); auto &&index : dataCube)
		for (auto &marker =
		         plot->markers.emplace_back(*plot->getOptions(),
		             dataCube,
		             stats,
		             mainIds,
		             subIds,
		             index,
		             first != set.end()
		                 && first->get().second == index.marker_id);
		     first != set.end()
		     && first->get().second == index.marker_id;
		     ++first)
			plot->markersInfo.insert({first->get().first,
			    Plot::MarkerInfo{Plot::MarkerInfoContent{marker}}});

	if (!std::ranges::is_sorted(plot->markers, {}, &Marker::idx))
		throw std::runtime_error(
		    "One of your series name or category contains control "
		    "character (possible tab/endline).");

	return Buckets{plot->markers};
}

std::vector<PlotBuilder::BucketInfo>
PlotBuilder::sortedBuckets(const Buckets &buckets, bool main) const
{
	std::vector<BucketInfo> sorted;

	for (auto &&bucket : buckets)
		for (auto &&[marker, idx] : bucket) {
			if (!marker.enabled) continue;

			// NOLINTNEXTLINE(misc-include-cleaner)
			auto it = std::ranges::lower_bound(sorted,
			    idx.itemId,
			    {},
			    &BucketInfo::index);
			if (it == sorted.end() || it->index != idx.itemId)
				it = sorted.emplace(it, idx.itemId, 0.0);

			it->size += marker.size.getCoord(
			    !plot->getOptions()->getOrientation());
		}

	if (main && plot->getOptions()->sort == Sort::byValue)
		std::ranges::stable_sort(sorted,
		    [](const BucketInfo &lhs, const BucketInfo &rhs)
		    {
			    return Math::Floating::less(lhs.size, rhs.size);
		    });

	if (main && plot->getOptions()->reverse)
		std::reverse(sorted.begin(), sorted.end());

	return sorted;
}

void PlotBuilder::addSpecLayout(Buckets &buckets)
{
	auto geometry = plot->getOptions()->geometry->value;
	if (auto &markers = plot->markers; isConnecting(geometry))
		Charts::TableChart::setupVector(markers, true);
	else if (!plot->getOptions()->isMeasure(ChannelId::size))
		Charts::TableChart::setupVector(markers);
	else if (!dataCube.empty()) {
		if (buckets.sort(&Marker::sizeId);
		    geometry == ShapeType::circle) {
			Charts::BubbleChartBuilder::setupVector(
			    *plot->getStyle().plot.marker.circleMaxRadius,
			    buckets);

			plot->keepAspectRatio = true;
		}
		else
			Charts::TreeMapBuilder::setupVector(buckets);
	}
}

void PlotBuilder::linkMarkers(Buckets &buckets)
{
	auto &&hasMarkerConnection =
	    linkMarkers(buckets.sort(&Marker::mainId),
	        plot->getOptions()->mainAxisType());
	std::ignore = linkMarkers(buckets.sort(&Marker::subId),
	    plot->getOptions()->subAxisType());

	if (hasMarkerConnection
	    && plot->getOptions()->geometry.get() == ShapeType::line
	    && !plot->getOptions()
	            ->getChannels()
	            .at(AxisId::x)
	            .hasMeasure()
	    && !plot->getOptions()
	            ->getChannels()
	            .at(AxisId::y)
	            .hasMeasure()) {
		plot->markerConnectionOrientation.emplace(
		    *plot->getOptions()->orientation.get());
	}
}

bool PlotBuilder::linkMarkers(const Buckets &buckets,
    AxisId axisIndex) const
{
	auto isMain = axisIndex == plot->getOptions()->mainAxisType();
	auto &&sorted = sortedBuckets(buckets, isMain);

	std::vector dimOffset(sorted.size(),
	    std::numeric_limits<double>::lowest());

	auto isAggregatable =
	    !plot->getOptions()->isMeasure(+axisIndex)
	    || (isMain && plot->getOptions()->isMeasure(+!axisIndex)
	        && plot->getOptions()->geometry.get()
	               == ShapeType::rectangle);

	if (isAggregatable) {
		double pre_neg{};
		double pre_pos{};
		for (std::size_t ix{}, max = sorted.size(); ix < max; ++ix) {
			auto &o = dimOffset[ix];
			for (const auto &bucket : buckets) {
				auto &&ids = std::ranges::views::values(bucket);
				auto sIx = sorted[ix].index;
				auto it = std::ranges::lower_bound(ids,
				    sIx,
				    {},
				    &Data::MarkerId::itemId);
				if (it == ids.end() || (*it).itemId != sIx) continue;

				auto &marker = **it.base().base().base();
				if (!marker.enabled) continue;
				o = std::max(o,
				    marker.size.getCoord(orientation(axisIndex)),
				    Math::Floating::less);
			}
			if (o == std::numeric_limits<double>::lowest()) o = 0.0;

			if (std::signbit(o))
				std::swap(o += pre_neg, pre_neg);
			else
				std::swap(o += pre_pos, pre_pos);
		}
	}

	bool hasConnection{};

	auto polar =
	    plot->getOptions()->coordSystem.get() == CoordSystem::polar;
	auto connecting =
	    isConnecting(plot->getOptions()->geometry.get());

	auto needPolarConnection = polar && connecting && isMain
	                        && plot->getOptions()->isHorizontal();

	for (const auto &bucket : buckets) {
		double prevPos{};
		for (auto i = 0U; i < sorted.size(); ++i) {
			auto idAct = sorted[i].index;
			auto &&ids = std::ranges::views::values(bucket);
			auto it = std::ranges::lower_bound(ids,
			    idAct,
			    {},
			    &Data::MarkerId::itemId);
			Marker *act = it == ids.end() || (*it).itemId != idAct
			                ? nullptr
			                : *it.base().base().base();

			auto iNext = (i + 1) % sorted.size();
			auto idNext = sorted[iNext].index;
			it = std::ranges::lower_bound(ids,
			    idNext,
			    {},
			    &Data::MarkerId::itemId);
			Marker *next = it == ids.end() || (*it).itemId != idNext
			                 ? nullptr
			                 : *it.base().base().base();

			if (act)
				prevPos =
				    act->position.getCoord(orientation(axisIndex)) +=
				    isAggregatable ? dimOffset[i] : prevPos;

			hasConnection |=
			    Marker::connectMarkers(iNext == 0 && act != next,
			        act,
			        next,
			        isMain,
			        needPolarConnection);
		}
	}
	return hasConnection;
}

void PlotBuilder::calcAxises(const Data::DataTable &dataTable)
{
	const auto &xrange =
	    plot->getOptions()->getHorizontalAxis().range;
	const auto &yrange = plot->getOptions()->getVerticalAxis().range;

	auto markerIt = plot->markers.begin();
	while (markerIt != plot->markers.end()
	       && !static_cast<bool>(markerIt->enabled))
		++markerIt;

	if (markerIt == plot->markers.end()) {
		stats.setIfRange(AxisId::x, xrange.getRange({0.0, 0.0}));
		stats.setIfRange(AxisId::y, xrange.getRange({0.0, 0.0}));
	}
	else {
		auto boundRect = markerIt->toRectangle();

		while (++markerIt != plot->markers.end()) {
			if (!markerIt->enabled) continue;
			boundRect = boundRect.boundary(markerIt->toRectangle());
		}

		plot->getOptions()->setAutoRange(
		    !std::signbit(boundRect.hSize().getMin()),
		    !std::signbit(boundRect.vSize().getMin()));

		boundRect.setHSize(xrange.getRange(boundRect.hSize()));
		boundRect.setVSize(yrange.getRange(boundRect.vSize()));

		for (auto &marker : plot->markers) {
			if (!boundRect.positive().intersects(
			        marker.toRectangle().positive()))
				marker.enabled = false;

			auto rect = marker.toRectangle();
			auto newRect = boundRect.normalize(rect);
			marker.fromRectangle(newRect);
		}

		stats.setIfRange(AxisId::x,
		    Math::Range<>::Raw(boundRect.left(), boundRect.right()));
		stats.setIfRange(AxisId::y,
		    Math::Range<>::Raw(boundRect.bottom(), boundRect.top()));
	}

	for (const AxisId &ch : {AxisId::x, AxisId::y})
		calcAxis(dataTable, ch);
}

void PlotBuilder::calcLegendAndLabel(const Data::DataTable &dataTable)
{
	if (auto &&legend = plot->getOptions()->legend.get()) {
		auto type{*legend};
		const auto &scale =
		    plot->getOptions()->getChannels().at(type);

		auto &calcLegend = plot->axises.create(type);
		auto isAutoTitle = scale.title.isAuto();
		if (scale.title) calcLegend.title = *scale.title;

		if (auto &&meas = scale.measure()) {
			if (plot->getOptions()->isMeasure(+type)) {
				if (isAutoTitle)
					calcLegend.title = dataCube.getName(*meas);
				calcLegend.measure = {std::get<0>(stats.at(type)),
				    meas->getColIndex(),
				    dataTable.getUnit(meas->getColIndex()),
				    scale.step.getValue()};
			}
		}
		else if (!scale.isEmpty()) {
			const auto &indices = std::get<1>(stats.at(type));
			auto merge =
			    type == LegendId::size
			    || (type == LegendId::lightness
			        && plot->getOptions()->dimLabelIndex(+type) == 0);
			for (std::uint32_t i{}, count{}; i < indices.size(); ++i)
				if (const auto &sliceIndex = indices[i]) {
					auto rangeId = static_cast<double>(i);
					std::optional<ColorBase> color;
					if (type == LegendId::color)
						color = ColorBase(i, 0.5);
					else if (type == LegendId::lightness) {
						rangeId = stats.lightness.rescale(rangeId);
						color = ColorBase(0U, rangeId);
					}

					if (calcLegend.dimension.add(*sliceIndex,
					        {rangeId, rangeId},
					        count,
					        color,
					        true,
					        merge))
						++count;
				}

			if (auto &&series = plot->getOptions()->labelSeries(type);
			    series && isAutoTitle && calcLegend.dimension.empty())
				calcLegend.title = series.value().getColIndex();
		}
	}

	if (auto &&meas = plot->getOptions()
	                      ->getChannels()
	                      .at(ChannelId::label)
	                      .measure())
		plot->axises.label = {
		    ::Anim::String{
		        std::string{dataTable.getUnit(meas->getColIndex())}},
		    ::Anim::String{meas->getColIndex()}};
}

void PlotBuilder::calcAxis(const Data::DataTable &dataTable,
    AxisId type)
{
	const auto &scale = plot->getOptions()->getChannels().at(type);
	if (scale.isEmpty()) return;

	auto &axis = plot->axises.at(type);

	auto isAutoTitle = scale.title.isAuto();
	if (scale.title) axis.title = *scale.title;

	if (plot->getOptions()->isMeasure(+type)) {
		const auto &meas = *scale.measure();
		if (isAutoTitle) axis.title = dataCube.getName(meas);

		if (type == plot->getOptions()->subAxisType()
		    && plot->getOptions()->align
		           == Base::Align::Type::stretch)
			axis.measure = {Math::Range<>::Raw(0, 100),
			    meas.getColIndex(),
			    "%",
			    scale.step.getValue()};
		else
			axis.measure = {std::get<0>(stats.at(type)),
			    meas.getColIndex(),
			    dataTable.getUnit(meas.getColIndex()),
			    scale.step.getValue()};
	}
	else {
		for (auto merge =
		         plot->getOptions()->dimLabelIndex(+type) == 0
		         && (type != plot->getOptions()->mainAxisType()
		             || plot->getOptions()->sort != Sort::byValue
		             || scale.dimensions().size() == 1);
		     const auto &marker : plot->markers) {
			if (!marker.enabled) continue;

			const auto &id =
			    type == plot->getOptions()->mainAxisType()
			        ? marker.mainId
			        : marker.subId;

			if (const auto &slice = id.label)
				axis.dimension.add(*slice,
				    marker.getSizeBy(type),
				    {},
				    {},
				    false,
				    merge);
		}
		if (auto &&series = plot->getOptions()->labelSeries(type);
		    !axis.dimension.setLabels(scale.step.getValue(1.0))
		    && series && isAutoTitle)
			axis.title = series.value().getColIndex();
		for (std::uint32_t pos{};
		     DimensionAxis::Item & i : axis.dimension.sortedItems())
			i.endPos = i.startPos =
			    DimensionAxis::Item::PosType{pos++};
	}
}

void PlotBuilder::addAlignment(const Buckets &subBuckets) const
{
	if (plot->getOptions()->isSplit()) return;

	auto &subAxisRange =
	    plot->axises.at(plot->getOptions()->subAxisType())
	        .measure.range;
	if (std::signbit(subAxisRange.getMin())
	    || std::signbit(subAxisRange.getMax()))
		return;

	if (plot->getOptions()->align == Base::Align::Type::none) return;

	if (plot->getOptions()->align == Base::Align::Type::center) {
		auto &&halfSize = subAxisRange.size() / 2.0;
		if (!Math::Floating::is_zero(halfSize))
			subAxisRange =
			    Math::Range<>::Raw(subAxisRange.getMin() - halfSize,
			        subAxisRange.getMax() - halfSize);
	}

	auto &&subAxis = plot->getOptions()->subAxisType();
	const Base::Align align{plot->getOptions()->align,
	    Math::Range<>::Raw(0.0, 1.0)};
	for (auto &&bucket : subBuckets) {
		Math::Range<> range;

		for (auto &&[marker, idx] : bucket)
			if (marker.enabled)
				range.include(marker.getSizeBy(subAxis));

		auto &&transform = align.getAligned(range) / range;

		for (auto &&[marker, idx] : bucket)
			marker.setSizeBy(subAxis,
			    marker.getSizeBy(subAxis) * transform);
	}
}

void PlotBuilder::addSeparation(const Buckets &subBuckets,
    const std::size_t &mainBucketSize) const
{
	if (!plot->getOptions()->isSplit()) return;

	auto align = plot->getOptions()->align;

	std::vector ranges{mainBucketSize, Math::Range<>::Raw({}, {})};
	std::vector<bool> anyEnabled(mainBucketSize);

	auto &&subAxis = plot->getOptions()->subAxisType();
	for (auto &&bucket : subBuckets)
		for (std::size_t i{}, prIx{}; auto &&[marker, idx] : bucket) {
			if (!marker.enabled) continue;
			(i += idx.itemId - std::exchange(prIx, idx.itemId)) %=
			    ranges.size();
			ranges[i].include(marker.getSizeBy(subAxis).size());
			anyEnabled[i] = true;
		}

	auto max = Math::Range<>::Raw({}, {});
	for (auto i = 0U; i < ranges.size(); ++i)
		if (anyEnabled[i]) max = max + ranges[i];

	auto splitSpace =
	    plot->getStyle()
	        .plot.getAxis(plot->getOptions()->subAxisType())
	        .spacing->get(max.getMax(),
	            plot->getStyle().calculatedSize());

	for (auto i = 1U; i < ranges.size(); ++i)
		ranges[i] = ranges[i] + ranges[i - 1].getMax()
		          + (anyEnabled[i - 1] ? splitSpace : 0);

	for (auto &&bucket : subBuckets)
		for (std::size_t i{}, prIx{}; auto &&[marker, idx] : bucket) {
			(i += idx.itemId - std::exchange(prIx, idx.itemId)) %=
			    ranges.size();
			marker.setSizeBy(subAxis,
			    Base::Align{align, ranges[i]}.getAligned(
			        marker.getSizeBy(subAxis)));
		}
}

void PlotBuilder::normalizeSizes()
{
	if (plot->getOptions()->geometry == ShapeType::circle
	    && !plot->getOptions()->getChannels().anyAxisSet())
		return;

	if (plot->getOptions()->geometry == ShapeType::circle
	    || plot->getOptions()->geometry == ShapeType::line) {
		Math::Range<> size;

		for (auto &marker : plot->markers) {
			if (std::isnan(marker.sizeFactor)) marker.enabled = false;
			if (!marker.enabled) continue;
			size.include(marker.sizeFactor);
		}

		size = plot->getOptions()
		           ->getChannels()
		           .at(ChannelId::size)
		           .range.getRange(size);

		for (auto &marker : plot->markers)
			marker.sizeFactor = size.getMax() == size.getMin()
			                      ? 0
			                      : size.normalize(marker.sizeFactor);

		stats.setIfRange(LegendId::size, size);
	}
	else
		for (auto &marker : plot->markers) marker.sizeFactor = 0;
}

void PlotBuilder::normalizeColors()
{
	auto &lightness = stats.lightness;
	Math::Range<> color;

	bool wasValidMarker{};
	for (auto &marker : plot->markers) {
		if (!marker.enabled) continue;
		auto &&cbase = marker.colorBase.get();
		if (!cbase.isDiscrete()) color.include(cbase.getPos());
		lightness.include(cbase.getLightness());
		wasValidMarker = true;
	}

	if (!wasValidMarker) {
		lightness.include(0.5);
		color.include(0);
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

	if (auto &&legend = plot->getOptions()->legend.get()) {
		switch (*legend) {
		case LegendId::color:
			stats.setIfRange(LegendId::color, color);
			break;
		case LegendId::lightness:
			stats.setIfRange(LegendId::lightness, lightness);
			break;
		default:;
		}
	}
}

}