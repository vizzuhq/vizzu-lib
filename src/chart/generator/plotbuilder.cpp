#include "plotbuilder.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <optional>
#include <ranges>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "base/anim/interpolated.h"
#include "base/math/floating.h"
#include "base/math/range.h"
#include "base/refl/auto_enum.h"
#include "base/text/naturalcmp.h"
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

#include "axis.h"
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

	auto &&buckets = generateMarkers();

	if (!plot->getOptions()->getChannels().anyAxisSet()) {
		addSpecLayout(buckets);
		normalizeSizes();
	}
	else {
		normalizeSizes();
		addAxisLayout(buckets, dataTable);
	}

	normalizeColors();
	calcLegendAndLabel(dataTable);
}

void PlotBuilder::addAxisLayout(Buckets &buckets,
    const Data::DataTable &dataTable)
{
	linkMarkers(buckets);
	calcAxises(dataTable, buckets);
	addAlignment(buckets, plot->getOptions()->mainAxisType());
	addAlignment(buckets.sort(&Marker::subId),
	    plot->getOptions()->subAxisType());
}

void PlotBuilder::initDimensionTrackers()
{
	for (auto type : Refl::enum_values<ChannelId>())
		if (auto &&ch = plot->getOptions()->getChannels().at(type);
		    !ch.hasMeasure())
			stats.tracked.at(type).emplace<1>();
}

Buckets PlotBuilder::generateMarkers()
{
	const auto &mainIds(plot->getOptions()->mainAxis().dimensions());
	auto subIds(plot->getOptions()->subAxis().dimensions());
	if (!dataCube.empty()) {
		if (plot->getOptions()->geometry == ShapeType::area)
			subIds.split_by(mainIds);

		plot->markers.reserve(dataCube.df->get_record_count());
	}

	struct CmpBySec
	{
		[[nodiscard]] bool operator()(
		    const std::pair<const Options::MarkerInfoId,
		        Marker::MarkerIndex> &lhs,
		    const std::pair<const Options::MarkerInfoId,
		        Marker::MarkerIndex> &rhs) const
		{
			return lhs.second < rhs.second;
		}
	};

	auto &&set =
	    std::multiset<std::reference_wrapper<
	                      const std::pair<const Options::MarkerInfoId,
	                          Marker::MarkerIndex>>,
	        CmpBySec>{plot->getOptions()->markersInfo.begin(),
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

std::vector<PlotBuilder::BucketSortInfo> PlotBuilder::sortedBuckets(
    const Buckets &buckets,
    AxisId axisIndex) const
{
	std::vector<BucketSortInfo> sorted;

	for (auto &&bucket : buckets)
		for (auto &&[marker, idx] : bucket) {
			if (!marker.enabled) continue;

			// NOLINTNEXTLINE(misc-include-cleaner)
			auto it = std::ranges::lower_bound(sorted,
			    idx.itemId,
			    {},
			    &BucketSortInfo::index);
			if (it == sorted.end() || it->index != idx.itemId)
				it = sorted.emplace(it,
				    idx.itemId,
				    0.0,
				    !(marker.*buckets.marker_id_get).label.empty()
				        ? std::make_optional(
				            DimensionAxis::mergedLabels(
				                (marker.*buckets.marker_id_get)
				                    .label))
				        : std::nullopt);

			it->size += marker.size.getCoord(
			    !plot->getOptions()->getOrientation());
		}

	switch (plot->getOptions()
	            ->getChannels()
	            .axisPropsAt(axisIndex)
	            .sort) {
	case Sort::byValue:
		std::ranges::stable_sort(sorted,
		    [](const BucketSortInfo &lhs, const BucketSortInfo &rhs)
		    {
			    return Math::Floating::less(lhs.size, rhs.size);
		    });
		break;
	case Sort::byLabel:
		std::ranges::stable_sort(sorted,
		    [](const BucketSortInfo &lhs, const BucketSortInfo &rhs)
		    {
			    if (rhs.label == std::nullopt
			        || lhs.label == std::nullopt)
				    return lhs.label != std::nullopt;
			    return Text::NaturalCmp{}(*lhs.label, *rhs.label);
		    });
		break;
	default: break;
	}

	if (plot->getOptions()
	        ->getChannels()
	        .axisPropsAt(axisIndex)
	        .reverse)
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
	auto &&sorted = sortedBuckets(buckets, axisIndex);

	std::vector dimOffset(sorted.size(),
	    std::numeric_limits<double>::lowest());

	auto isAggregatable =
	    !plot->getOptions()->isMeasure(+axisIndex)
	    || (isMain && plot->getOptions()->isMeasure(+!axisIndex)
	        && plot->getOptions()->geometry.get()
	               == ShapeType::rectangle);

	auto &&subAxisDims = plot->getOptions()->subAxis().dimensions();
	auto isSubAggregatable =
	    !isMain
	    && plot->getOptions()->geometry.get() == ShapeType::rectangle
	    && !plot->getOptions()->mainAxis().dimensions().contains_any(
	        subAxisDims.begin(),
	        {});

	if (isAggregatable) {
		double pre_neg{};
		double pre_pos{};
		for (std::size_t ix{}, max = sorted.size(); ix < max; ++ix) {
			auto &o = dimOffset[ix];
			for (const auto &bucket : buckets) {
				auto &&ids = std::views::values(bucket);
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
		std::array<double, 2> prevPositions{};
		for (auto i = 0U; i < sorted.size(); ++i) {
			auto idAct = sorted[i].index;
			auto &&ids = std::views::values(bucket);
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

			if (act) {
				auto &apos =
				    act->position.getCoord(orientation(axisIndex));
				if (!std::isfinite(apos)) apos = 0;
				if (isAggregatable)
					apos += dimOffset[i];
				else {
					auto &pp = prevPositions[isSubAggregatable
					                         && std::signbit(apos)];
					pp = apos += pp;
				}
			}

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

void PlotBuilder::calcAxises(const Data::DataTable &dataTable,
    Buckets &buckets)
{
	auto mainAxis = plot->getOptions()->mainAxisType();
	auto &&subRanges = addSeparation(buckets, !mainAxis);

	auto &&mainRanges =
	    addSeparation(buckets.sort(&Marker::mainId), mainAxis);

	auto mainBoundRect = plot->getMarkersBounds(mainAxis);
	auto subBoundRect = plot->getMarkersBounds(!mainAxis);

	plot->getOptions()->setAutoRange(
	    !std::signbit(
	        (mainAxis == AxisId::x ? mainBoundRect : subBoundRect)
	            .min),
	    !std::signbit(
	        (mainAxis == AxisId::x ? subBoundRect : mainBoundRect)
	            .min));

	mainBoundRect =
	    plot->getOptions()->mainAxis().range.getRange(mainBoundRect);
	subBoundRect =
	    plot->getOptions()->subAxis().range.getRange(subBoundRect);

	for (auto &&[axis, needRanges, boundSize] :
	    {std::tuple{mainAxis, mainRanges.empty(), mainBoundRect},
	        {!mainAxis, subRanges.empty(), subBoundRect}}) {
		if (!needRanges) continue;

		for (auto &marker : plot->markers) {
			auto &&markerSize = marker.getSizeBy(axis);
			if (!boundSize.positive().intersects(
			        markerSize.positive()))
				marker.enabled = false;

			marker.setSizeBy(axis,
			    {boundSize.rescale(markerSize.min, 0.0),
			        boundSize.rescale(markerSize.max, 0.0)});
		}
		stats.setIfRange(axis, boundSize);
	}

	for (auto &&[ch, ranges] :
	    {std::pair{mainAxis, &mainRanges}, {!mainAxis, &subRanges}}) {
		calcAxis(dataTable, ch);
		if (ranges->empty()) continue;

		auto &axis = plot->axises.at(ch);
		for (auto &&range : *ranges)
			axis.parts.insert({range.index,
			    {1.0, range.atRange, range.containsValues}});
	}
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
				    auto{meas->getColIndex()},
				    dataTable.get_series_info(meas->getColIndex(),
				        "unit"),
				    {1}};
			}
		}
		else if (!scale.isEmpty()) {
			auto merge = type == LegendId::size
			          || (type == LegendId::lightness
			              && plot->getOptions()->dimLabelIndex(+type)
			                     == std::vector{std::size_t{}});
			for (std::uint32_t count{}; const auto &[i, label] :
			     std::get<1>(stats.at(type))) {
				auto rangeId = static_cast<double>(i);
				std::optional<ColorBase> color;
				if (type == LegendId::color)
					color = ColorBase(i, 0.5);
				else if (type == LegendId::lightness) {
					rangeId = stats.lightness.rescale(rangeId);
					color = ColorBase(0U, rangeId);
				}

				if (calcLegend.dimension.add(label,
				        {rangeId, rangeId},
				        count,
				        color,
				        true,
				        merge))
					++count;
				calcLegend.dimension.hasMarker = true;
			}

			if (auto &&series = plot->getOptions()->labelSeries(type);
			    !series.empty() && isAutoTitle
			    && calcLegend.dimension.empty())
				calcLegend.title = std::string{std::from_range,
				    series
				        | std::views::transform(
				            [](const Data::SeriesIndex &series)
				            {
					            return ", " + series.getColIndex();
				            })
				        | std::views::join | std::views::drop(2)};
		}
	}

	if (auto &&meas = plot->getOptions()
	                      ->getChannels()
	                      .at(ChannelId::label)
	                      .measure()) {

		auto mainAxisType = plot->getOptions()->mainAxisType();
		auto subAxisType = plot->getOptions()->subAxisType();
		auto markerLabelsUnitPercent =
		    plot->getStyle().plot.marker.label.unit
		        == Styles::MarkerLabel::Unit::percent
		    && ((plot->getOptions()
		                    ->getChannels()
		                    .axisPropsAt(mainAxisType)
		                    .align
		                == Base::Align::Type::stretch
		            && plot->getOptions()
		                       ->getChannels()
		                       .at(mainAxisType)
		                       .measure()
		                   == *meas
		            && !plot->getOptions()->isSplit(mainAxisType))
		        || (plot->getOptions()
		                    ->getChannels()
		                    .axisPropsAt(subAxisType)
		                    .align
		                == Base::Align::Type::stretch
		            && plot->getOptions()
		                       ->getChannels()
		                       .at(subAxisType)
		                       .measure()
		                   == *meas
		            && !plot->getOptions()->isSplit(subAxisType)));

		plot->axises.label = {
		    ::Anim::String{std::string{
		        markerLabelsUnitPercent
		            ? "%"
		            : dataTable.get_series_info(meas->getColIndex(),
		                "unit")}},
		    ::Anim::String{meas->getColIndex()}};
	}
}

void PlotBuilder::calcAxis(const Data::DataTable &dataTable,
    AxisId type)
{
	const auto &scale = plot->getOptions()->getChannels().at(type);

	auto &axisProps =
	    plot->getOptions()->getChannels().axisPropsAt(type);

	auto &axis = plot->axises.at(type);

	auto isAutoTitle = scale.title.isAuto();
	if (scale.title) axis.title = *scale.title;

	if (plot->getOptions()->isMeasure(+type)) {
		const auto &meas = *scale.measure();
		if (isAutoTitle) axis.title = dataCube.getName(meas);

		if (axisProps.align == Base::Align::Type::stretch)
			axis.measure = {{0, 100},
			    auto{meas.getColIndex()},
			    "%",
			    axisProps.step.getValue()};
		else
			axis.measure = {std::get<0>(stats.at(type)),
			    auto{meas.getColIndex()},
			    dataTable.get_series_info(meas.getColIndex(), "unit"),
			    axisProps.step.getValue()};
	}
	else {
		for (auto merge = axisProps.sort == Sort::byLabel
		               || (plot->getOptions()->dimLabelIndex(+type)
		                       == std::vector{std::size_t{}}
		                   && (axisProps.sort == Sort::none
		                       || scale.dimensions().size() == 1));
		     const auto &marker : plot->markers) {
			if (!marker.enabled) continue;

			const auto &id =
			    type == plot->getOptions()->mainAxisType()
			        ? marker.mainId
			        : marker.subId;

			if (const auto &slice = id.label; !slice.empty())
				axis.dimension.add(slice,
				    marker.getSizeBy(type),
				    {},
				    {},
				    false,
				    merge);

			axis.dimension.hasMarker = true;
		}
		if (auto &&series = plot->getOptions()->labelSeries(type);
		    !axis.dimension.setLabels(axisProps.step.getValue(1.0))
		    && !series.empty() && isAutoTitle)
			axis.title = std::string{std::from_range,
			    series
			        | std::views::transform(
			            [](const Data::SeriesIndex &series)
			            {
				            return ", " + series.getColIndex();
			            })
			        | std::views::join | std::views::drop(2)};
		for (std::uint32_t pos{};
		     DimensionAxis::Item & i : axis.dimension.sortedItems())
			i.endPos = i.startPos =
			    DimensionAxis::Item::PosType{pos++};
	}
}

void PlotBuilder::addAlignment(const Buckets &buckets,
    AxisId axisIndex) const
{
	if (plot->getOptions()->isSplit(axisIndex)) return;

	auto &axisRange = plot->axises.at(axisIndex).measure.range;
	if (std::signbit(axisRange.min) || std::signbit(axisRange.max))
		return;

	const auto &axisProps =
	    plot->getOptions()->getChannels().axisPropsAt(axisIndex);

	if (axisProps.align == Base::Align::Type::none) return;

	if (axisProps.align == Base::Align::Type::center) {
		auto &&halfSize = axisRange.middle();
		axisRange = {axisRange.min - halfSize,
		    axisRange.max - halfSize};
	}

	auto &&axisMeasure =
	    plot->getOptions()->getChannels().at(axisIndex).measure();
	auto markerLabelsUnitPercent =
	    plot->getStyle().plot.marker.label.unit
	        == Styles::MarkerLabel::Unit::percent
	    && axisProps.align == Base::Align::Type::stretch
	    && axisMeasure.has_value()
	    && axisMeasure
	           == plot->getOptions()
	                  ->getChannels()
	                  .at(ChannelId::label)
	                  .measure();

	const Base::Align align{axisProps.align, {0.0, 1.0}};
	for (auto &&bucket : buckets) {
		Math::Range<> range;

		for (auto &&[marker, idx] : bucket)
			if (marker.enabled)
				range.include(marker.getSizeBy(axisIndex));

		auto &&transform = align.getAligned(range) / range;

		for (auto &&[marker, idx] : bucket) {
			auto &&newRange = marker.getSizeBy(axisIndex) * transform;
			marker.setSizeBy(axisIndex, newRange);
			if (markerLabelsUnitPercent)
				marker.label->value.value.emplace(
				    newRange.size() * 100);
		}
	}
}

std::vector<PlotBuilder::BucketSeparationInfo>
PlotBuilder::addSeparation(const Buckets &buckets, AxisId axisIndex)
{
	if (!plot->getOptions()->isSplit(axisIndex)) return {};

	const auto &axisProps =
	    plot->getOptions()->getChannels().axisPropsAt(axisIndex);
	auto align = axisProps.align;

	std::vector<BucketSeparationInfo> res;

	for (auto &&bucket : buckets)
		for (auto &&[marker, idx] : bucket) {
			if (!marker.enabled) continue;

			auto it =
			    std::lower_bound(res.begin(), res.end(), idx.itemId);
			if (it == res.end() || it->itemId != idx.itemId) {
				it = res.insert(it, {idx.itemId});
			}

			auto &resItem = *it;
			if (resItem.index.empty() && !idx.label.empty())
				resItem.index = idx.label;

			resItem.containsValues.include(
			    marker.getSizeBy(axisIndex).size());
		}

	auto max = Math::Range<>{{}, {}};
	auto maxRange = Math::Range<>{{}, {}};
	for (auto &resItem : res) {
		auto onlyPositive = !std::signbit(resItem.containsValues.min);
		plot->getOptions()->setAutoRange(onlyPositive,
		    onlyPositive,
		    true);

		resItem.containsValues =
		    plot->getOptions()
		        ->getChannels()
		        .at(axisIndex)
		        .range.getRange(resItem.containsValues);

		max = max + resItem.containsValues;
		maxRange.include(resItem.containsValues);
	}

	auto splitSpace =
	    plot->getStyle().plot.getAxis(axisIndex).spacing->get(
	        max.size(),
	        plot->getStyle().calculatedSize());

	double onMax = 0.0;
	bool first = true;
	for (auto &&resItem : res) {
		if (first)
			first = false;
		else
			onMax += splitSpace;
		resItem.atRange = resItem.containsValues + onMax
		                - resItem.containsValues.min;
		onMax += resItem.containsValues.size();
	}

	if (plot->getOptions()->coordSystem == CoordSystem::polar
	    && axisIndex == AxisId::x && !first)
		onMax += splitSpace;

	for (auto &&bucket : buckets)
		for (auto &&[marker, idx] : bucket) {

			auto it =
			    std::lower_bound(res.begin(), res.end(), idx.itemId);

			Math::Range<> atRange{};
			Math::Range<> containsValues{};
			if (it == res.end()) {
				if (it != res.begin()) {
					--it;
					atRange = {it->atRange.max, it->atRange.max};
					containsValues = {it->containsValues.max,
					    it->containsValues.max};
				}
			}
			else if (it->itemId != idx.itemId) {
				if (it == res.begin()) {
					atRange = {it->atRange.min, it->atRange.min};
					containsValues = {it->containsValues.min,
					    it->containsValues.min};
				}
				else {
					auto prev = std::prev(it);
					auto centerRange =
					    std::midpoint(prev->atRange.max,
					        it->atRange.min);
					auto centerValues =
					    std::midpoint(prev->containsValues.max,
					        it->containsValues.min);
					atRange = {centerRange, centerRange};
					containsValues = {centerValues, centerValues};
				}
			}
			else {
				atRange = it->atRange;
				containsValues = it->containsValues;
			}

			// auto buc = res[idx.itemId];
			auto markerSize = marker.getSizeBy(axisIndex);

			marker.setSizeBy(axisIndex,
			    (Base::Align{align,
			         atRange - atRange.min + containsValues.min}
			            .getAligned(markerSize - markerSize.min)
			        + atRange.min - containsValues.min)
			        / onMax);
		}

	auto alignedRange = maxRange;
	if (align == Base::Align::Type::center) {
		auto &&halfSize = maxRange.middle();
		alignedRange = {maxRange.min - halfSize,
		    maxRange.max - halfSize};
	}

	for (auto &resItem : res) {
		auto aligned =
		    Base::Align{align, resItem.containsValues}.getAligned(
		        maxRange);

		resItem.atRange = (aligned + resItem.atRange.min
		                      - resItem.containsValues.min)
		                / onMax;

		resItem.containsValues = {
		    aligned.rescale(resItem.containsValues.min),
		    aligned.rescale(resItem.containsValues.max)};
	}

	stats.setIfRange(axisIndex, alignedRange);
	return res;
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
			marker.sizeFactor = size.max == size.min
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
