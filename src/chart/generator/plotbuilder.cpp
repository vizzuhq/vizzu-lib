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

	if (!plot->options->getChannels().anyAxisSet())
		addSpecLayout(subBuckets);
	else
		addAxisLayout(subBuckets, mainBucketSize, dataTable);

	normalizeColors();
	normalizeSizes();
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
	for (auto *tracks = stats.tracked.data();
	     const auto &ch : plot->options->getChannels())
		if (auto &track = *tracks++; ch.isDimension())
			track.emplace<1>(
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

	std::multimap<Marker::MarkerIndex, Options::MarkerInfoId> map;
	for (auto &&[ix, mid] : plot->getOptions()->markersInfo)
		map.emplace(mid, ix);

	for (auto first = map.begin(); auto &&index : dataCube)
		for (auto &marker =
		         plot->markers.emplace_back(*plot->getOptions(),
		             dataCube,
		             stats,
		             mainIds,
		             subIds,
		             index,
		             map.contains(index.marker_id));
		     first != map.end() && first->first == marker.idx;
		     ++first)
			plot->markersInfo.insert({first->second,
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
			    !plot->getOptions()->isHorizontal());
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
	auto geometry = plot->getOptions()->geometry.values[0].value;
	if (auto &markers = plot->markers; isConnecting(geometry))
		Charts::TableChart::setupVector(markers, true);
	else if (plot->getOptions()
	             ->getChannels()
	             .at(ChannelId::size)
	             .isDimension())
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
	    linkMarkers(buckets.sort(&Marker::mainId), true);
	std::ignore = linkMarkers(buckets.sort(&Marker::subId), false);

	if (hasMarkerConnection
	    && plot->getOptions()->geometry.get() == ShapeType::line
	    && plot->getOptions()
	           ->getChannels()
	           .at(AxisId::x)
	           .isDimension()
	    && plot->getOptions()
	           ->getChannels()
	           .at(AxisId::y)
	           .isDimension()) {
		plot->markerConnectionOrientation.emplace(
		    *plot->getOptions()->orientation.get());
	}
}

bool PlotBuilder::linkMarkers(const Buckets &buckets, bool main) const
{
	auto &&sorted = sortedBuckets(buckets, main);

	std::vector dimOffset(sorted.size(),
	    std::numeric_limits<double>::lowest());

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
				    marker.size.*coord,
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

	auto needPolarConnection =
	    polar && connecting && main && horizontal;

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
				prevPos = act->position.*coord +=
				    isAggregatable ? dimOffset[i] : prevPos;

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
	if (auto &&legend = plot->options->legend.get()) {
		auto type{*legend};
		const auto &scale =
		    plot->getOptions()->getChannels().at(type);

		auto &calcLegend = plot->axises.create(type);
		auto isAutoTitle = scale.title.isAuto();
		if (scale.title) calcLegend.title = *scale.title;

		if (auto &&meas = scale.measure()) {
			if (isAutoTitle)
				calcLegend.title = dataCube.getName(*meas);
			calcLegend.measure = {std::get<0>(stats.at(type)),
			    meas->getColIndex(),
			    dataTable.getUnit(meas->getColIndex()),
			    scale.step.getValue()};
		}
		else if (!scale.isEmpty()) {
			const auto &indices = std::get<1>(stats.at(type));
			auto merge = type == LegendId::size
			          || (type == LegendId::lightness
			              && scale.labelLevel == 0);
			for (std::uint32_t i{}, count{}; i < indices.size(); ++i)
				if (const auto &sliceIndex = indices[i]; sliceIndex) {

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

			if (auto &&series = scale.labelSeries();
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

	if (auto &&meas = scale.measure()) {
		if (isAutoTitle) axis.title = dataCube.getName(*meas);

		if (type == plot->getOptions()->subAxisType()
		    && plot->getOptions()->align
		           == Base::Align::Type::stretch)
			axis.measure = {Math::Range<>::Raw(0, 100),
			    meas->getColIndex(),
			    "%",
			    scale.step.getValue()};
		else
			axis.measure = {std::get<0>(stats.at(type)),
			    meas->getColIndex(),
			    dataTable.getUnit(meas->getColIndex()),
			    scale.step.getValue()};
	}
	else {
		for (auto merge =
		         scale.labelLevel == 0
		         && ((type == AxisId::x)
		                 != plot->getOptions()->isHorizontal()
		             || plot->getOptions()->sort != Sort::byValue
		             || scale.dimensions().size() == 1);
		     const auto &marker : plot->markers) {
			if (!marker.enabled) continue;

			const auto &id =
			    (type == AxisId::x)
			            == plot->getOptions()->isHorizontal()
			        ? marker.mainId
			        : marker.subId;

			if (const auto &slice = id.label)
				axis.dimension.add(*slice,
				    marker.getSizeBy(type == AxisId::x),
				    {},
				    {},
				    false,
				    merge);
		}
		if (auto &&series = scale.labelSeries();
		    !axis.dimension.setLabels(scale.step.getValue(1.0))
		    && series && isAutoTitle)
			axis.title = series.value().getColIndex();
	}
}

void PlotBuilder::addAlignment(const Buckets &subBuckets) const
{
	if (static_cast<bool>(plot->getOptions()->split)) return;

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

	auto &&vectical = !plot->getOptions()->isHorizontal();
	const Base::Align align{plot->getOptions()->align,
	    Math::Range(0.0, 1.0)};
	for (auto &&bucket : subBuckets) {
		Math::Range<> range;

		for (auto &&[marker, idx] : bucket)
			if (marker.enabled)
				range.include(marker.getSizeBy(vectical));

		auto &&transform = align.getAligned(range) / range;

		for (auto &&[marker, idx] : bucket)
			marker.setSizeBy(vectical,
			    marker.getSizeBy(vectical) * transform);
	}
}

void PlotBuilder::addSeparation(const Buckets &subBuckets,
    const std::size_t &mainBucketSize) const
{
	if (plot->getOptions()->split) {
		auto align = plot->getOptions()->align;

		std::vector ranges{mainBucketSize,
		    Math::Range<>::Raw({}, {})};
		std::vector<bool> anyEnabled(mainBucketSize);

		auto &&vertical = !plot->getOptions()->isHorizontal();
		for (auto &&bucket : subBuckets)
			for (std::size_t i{}, prIx{};
			     auto &&[marker, idx] : bucket) {
				(i += idx.itemId - std::exchange(prIx, idx.itemId)) %=
				    ranges.size();
				if (marker.enabled) {
					ranges[i].include(
					    marker.getSizeBy(vertical).size());
					anyEnabled[i] = true;
				}
			}

		auto max = Math::Range(0.0, 0.0);
		for (auto i = 0U; i < ranges.size(); ++i)
			if (anyEnabled[i]) max = max + ranges[i];

		for (auto i = 1U; i < ranges.size(); ++i)
			ranges[i] = ranges[i] + ranges[i - 1].getMax()
			          + (anyEnabled[i - 1] ? max.getMax() / 15 : 0);

		for (auto &&bucket : subBuckets)
			for (std::size_t i{}, prIx{};
			     auto &&[marker, idx] : bucket) {
				(i += idx.itemId - std::exchange(prIx, idx.itemId)) %=
				    ranges.size();
				marker.setSizeBy(vertical,
				    Base::Align{align, ranges[i]}.getAligned(
				        marker.getSizeBy(vertical)));
			}
	}
}

void PlotBuilder::normalizeSizes()
{
	if (plot->getOptions()->geometry == ShapeType::circle
	    && !plot->options->getChannels().anyAxisSet())
		return;

	if (plot->getOptions()->geometry == ShapeType::circle
	    || plot->getOptions()->geometry == ShapeType::line) {
		Math::Range<> size;

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

	if (auto &&legend = plot->options->legend.get()) {
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