#include <limits>
#include <numeric>

#include "base/anim/interpolated.h"
#include "base/conv/numtostr.h"
#include "base/io/log.h"
#include "base/math/range.h"
#include "chart/speclayout/speclayout.h"
#include "data/datacube/datacube.h"

#include "plot.h"

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
	     iter1++, iter2++) {
		if (iter1->first != iter2->first)
			throw std::logic_error("invalid map operation");
		result.insert(std::make_pair(iter1->first,
		    interpolate(iter1->second, iter2->second, factor)));
	}
	return result;
}

Plot::MarkerInfoContent::MarkerInfoContent()
{
	markerId = (uint64_t)-1;
}

Plot::MarkerInfoContent::MarkerInfoContent(const Marker &marker,
    Data::DataCube *dataCube)
{
	const auto &index = marker.index;
	if (dataCube && dataCube->getTable() && index.size() != 0) {
		markerId = marker.idx;
		const auto &dataCellInfo = dataCube->cellInfo(index);
		const auto &table = *dataCube->getTable();
		for (auto &cat : dataCellInfo.categories) {
			auto series = cat.first;
			auto category = cat.second;
			auto colIndex = series.getColIndex();
			auto value =
			    table.getInfo(colIndex).categories()[category];
			content.push_back(
			    std::make_pair(series.toString(table), value));
		}
		for (auto &val : dataCellInfo.values) {
			auto series = val.first;
			auto value = val.second;
			Conv::NumberToString conv;
			conv.fractionDigitCount = 3;
			content.push_back(
			    std::make_pair(series.toString(table), conv(value)));
		}
	}
	else
		markerId = (uint64_t)-1;
}

Plot::MarkerInfoContent::operator bool() const
{
	return markerId != (uint64_t)-1;
}

bool Plot::MarkerInfoContent::operator==(
    const MarkerInfoContent &op) const
{
	return markerId == op.markerId;
}

Plot::Plot(PlotOptionsPtr options, const Plot &other) :
    dataTable(other.getTable()),
    options(std::move(options))
{
	anySelected = other.anySelected;
	axises = other.axises;
	guides = other.guides;
	dimensionAxises = other.dimensionAxises;
	anyAxisSet = other.anyAxisSet;
	style = other.style;
	keepAspectRatio = other.keepAspectRatio;
	markersInfo = other.markersInfo;
}

Plot::Plot(const Data::DataTable &dataTable,
    PlotOptionsPtr opts,
    Styles::Chart style,
    bool setAutoParams) :
    dataTable(dataTable),
    options(std::move(opts)),
    style(std::move(style)),
    dataCube(dataTable,
        options->getChannels().getDataCubeOptions(),
        options->dataFilter.get()),
    stats(options->getChannels(), dataCube)
{
	if (setAutoParams) options->setAutoParameters();

	anySelected = false;
	anyAxisSet = options->getChannels().anyAxisSet();

	generateMarkers(dataCube, dataTable);
	generateMarkersInfo();

	SpecLayout specLayout(*this);
	auto gotSpecLayout = specLayout.addIfNeeded();

	if (gotSpecLayout) {
		calcDimensionAxises(dataTable);
		normalizeColors();
		if (options->shapeType.get() != ShapeType::Circle)
			normalizeSizes();
		calcAxises(dataTable);
	}
	else {
		addSeparation();
		normalizeXY();
		calcDimensionAxises(dataTable);
		normalizeSizes();
		normalizeColors();
		calcAxises(dataTable);
		addAlignment();
		recalcStackedLineChart();
	}

	guides.init(axises, *options);
}

void Plot::detachOptions()
{
	options = std::make_shared<Gen::Options>(*options);
}

bool Plot::isEmpty() const
{
	return options->getChannels().isEmpty();
}

void Plot::generateMarkers(const Data::DataCube &dataCube,
    const Data::DataTable &table)
{
	for (auto it = dataCube.getData().begin();
	     it != dataCube.getData().end();
	     ++it) {
		auto itemIndex = markers.size();

		markers.emplace_back(*options,
		    style,
		    dataCube,
		    table,
		    stats,
		    it.getIndex(),
		    itemIndex);

		auto &marker = markers[itemIndex];

		mainBuckets[marker.mainId.seriesId][marker.mainId.itemId] =
		    itemIndex;
		subBuckets[marker.subId.seriesId][marker.subId.itemId] =
		    itemIndex;
	}
	clearEmptyBuckets(mainBuckets, true);
	clearEmptyBuckets(subBuckets, false);
	linkMarkers(mainBuckets, true);
	linkMarkers(subBuckets, false);
}

void Plot::generateMarkersInfo()
{
	for (auto &mi : options->markersInfo.get()) {
		auto &marker = markers[mi.second];
		markersInfo.insert(
		    std::make_pair(mi.first, MarkerInfo{marker, &dataCube}));
	}
}

std::vector<std::pair<uint64_t, double>>
Plot::sortedBuckets(const Buckets &buckets, bool main)
{
	size_t maxBucketSize = 0;
	for (const auto &pair : buckets)
		if (pair.second.size() > maxBucketSize)
			maxBucketSize = pair.second.size();

	std::vector<std::pair<uint64_t, double>> sorted;
	sorted.resize(maxBucketSize);
	for (auto &pair : sorted) pair.second = 0;

	for (const auto &pair : buckets) {
		const auto &bucket = pair.second;

		for (const auto &id : bucket) {
			auto &marker = markers[id.second];
			auto horizontal = (bool)options->horizontal.get();
			auto size = marker.size.getCoord(!horizontal);
			sorted[id.first].first = id.first;
			sorted[id.first].second += size;
		}
	}

	if (main && options->sorted.get()) {
		std::sort(sorted.begin(),
		    sorted.end(),
		    [=](const std::pair<uint64_t, double> &a,
		        const std::pair<uint64_t, double> &b)
		    {
			    return a.second < b.second;
		    });
	}
	if (main && options->reverse.get()) {
		std::reverse(sorted.begin(), sorted.end());
	}

	return sorted;
}

void Plot::clearEmptyBuckets(const Buckets &buckets, bool main)
{
	for (const auto &pair : buckets) {
		const auto &bucket = pair.second;

		bool enabled = false;

		for (const auto &id : bucket) {
			auto &marker = markers[id.second];
			enabled |= (bool)marker.enabled;
		}

		if (!enabled)
			for (const auto &id : bucket) {
				auto &marker = markers[id.second];
				marker.resetSize(
				    (bool)options->horizontal.get() == !main);
			}
	}
}

void Plot::linkMarkers(const Buckets &buckets, bool main)
{
	auto sorted = sortedBuckets(buckets, main);

	for (const auto &pair : buckets) {
		const auto &bucket = pair.second;

		for (auto i = 0u; i < sorted.size(); i++) {
			auto idAct = sorted[i].first;
			auto indexAct = bucket.at(idAct);
			auto &act = markers[indexAct];
			auto iNext = (i + 1) % sorted.size();
			auto idNext = sorted[iNext].first;
			auto indexNext = bucket.at(idNext);
			act.setNextMarker(iNext,
			    &markers[indexNext],
			    (bool)options->horizontal.get() == main,
			    main);
		}
	}
}

void Plot::normalizeXY()
{
	if (markers.empty()) {
		stats.channels[ChannelId::x].range =
		    Math::Range<double>(0.0, 0.0);
		stats.channels[ChannelId::y].range =
		    Math::Range<double>(0.0, 0.0);
		return;
	}

	auto boundRect = markers.front().toRectangle();

	for (auto &marker : markers)
		boundRect = boundRect.boundary(marker.toRectangle());

	options->setAutoRange(boundRect.positive().hSize().getMin() >= 0,
	    boundRect.positive().vSize().getMin() >= 0);

	auto xrange = options->getHorizontalAxis().range.get();
	auto yrange = options->getVeritalAxis().range.get();

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

void Plot::calcAxises(const Data::DataTable &dataTable)
{
	for (auto i = 0u; i < ChannelId::EnumInfo::count(); i++) {
		auto id = ChannelId(i);
		axises.at(id) = calcAxis(id, dataTable);
	}
}

Axis Plot::calcAxis(ChannelId type, const Data::DataTable &dataTable)
{
	const auto &scale = options->getChannels().at(type);
	if (!scale.isEmpty() && !scale.isPseudoDimension()) {
		auto title = scale.title.get() == "auto"
		               ? scale.measureName(dataTable)
		           : scale.title.get() == "null" ? std::string()
		                                         : scale.title.get();

		if (type == options->subAxisType()
		    && options->alignType.get() == Base::Align::Fit) {
			return Axis(Math::Range<double>(0, 100),
			    title,
			    "%",
			    scale.step.get().getValue());
		}
		else {
			auto unit =
			    dataTable.getInfo(scale.measureId()->getColIndex())
			        .getUnit();
			return Axis(stats.channels[type].range,
			    title,
			    unit,
			    scale.step.get().getValue());
		}
	}
	else
		return Axis();
}

void Plot::calcDimensionAxises(const Data::DataTable &table)
{
	for (auto i = 0u; i < ChannelId::EnumInfo::count(); i++)
		calcDimensionAxis(ChannelId(i), table);
}

void Plot::calcDimensionAxis(ChannelId type,
    const Data::DataTable &table)
{
	auto &axis = dimensionAxises.at(type);
	auto &scale = options->getChannels().at(type);
	auto dim = scale.labelLevel.get();

	if (scale.dimensionIds().empty() || !scale.isPseudoDimension())
		return;

	axis.title =
	    scale.title.get() == "auto" || scale.title.get() == "null"
	        ? std::string()
	        : scale.title.get();

	if (type == ChannelId::x || type == ChannelId::y) {
		for (auto marker : markers) {
			auto &id =
			    (type == ChannelId::x) == options->horizontal.get()
			        ? marker.mainId
			        : marker.subId;

			auto &slice = id.itemSliceIndex;

			if (!slice.empty() && dim >= 0 && dim < slice.size()
			    && dim == floor(dim)) {
				auto index = slice[dim];
				auto range = marker.getSizeBy(type == ChannelId::x);
				axis.add(index,
				    id.itemId,
				    range,
				    (double)marker.enabled);
			}
		}
	}
	else {
		const auto &indices = stats.channels[type].usedIndices;

		auto count = 0;
		for (auto i = 0u; i < indices.size(); i++) {
			const auto &sliceIndex = indices[i];

			if (!sliceIndex.empty() && dim >= 0
			    && dim < sliceIndex.size() && dim == floor(dim)) {
				auto index = sliceIndex[dim];
				auto range = Math::Range<double>(count, count);
				auto inserted = axis.add(index, i, range, true);
				if (inserted) count++;
			}
		}
	}
	axis.setLabels(dataCube, table);
}

void Plot::addAlignment()
{
	if ((bool)options->splitted.get()) return;

	auto &axis = axises.at(options->subAxisType());
	if (axis.range.getMin() < 0) return;

	if (options->alignType.get() == Base::Align::None) return;

	for (auto &bucketIt : subBuckets) {
		Math::Range<double> range;

		for (auto &itemIt : bucketIt.second) {
			auto &marker = markers[itemIt.second];
			auto size =
			    marker.getSizeBy(!(bool)options->horizontal.get());
			range.include(size);
		}

		Base::Align aligner(options->alignType.get(),
		    Math::Range(0.0, 1.0));
		auto transform = aligner.getAligned(range) / range;

		for (auto &itemIt : bucketIt.second) {
			auto &marker = markers[itemIt.second];
			auto newRange =
			    marker.getSizeBy(!(bool)options->horizontal.get())
			    * transform;
			marker.setSizeBy(!(bool)options->horizontal.get(),
			    newRange);
		}
	}
}

void Plot::addSeparation()
{
	if ((bool)options->splitted.get()) {
		auto align = options->alignType.get() == Base::Align::None
		               ? Base::Align::Min
		               : options->alignType.get();

		std::vector<Math::Range<double>> ranges(mainBuckets.size(),
		    Math::Range(0.0, 0.0));
		std::vector<bool> anyEnabled(mainBuckets.size(), false);

		for (auto &bucketIt : subBuckets) {
			auto i = 0u;
			for (auto &itemIt : bucketIt.second) {
				auto &marker = markers[itemIt.second];
				auto size =
				    marker.getSizeBy(!(bool)options->horizontal.get())
				        .size();
				ranges[i].include(size);
				if ((double)marker.enabled > 0) anyEnabled[i] = true;
				i++;
			}
		}

		auto max = Math::Range(0.0, 0.0);
		for (auto i = 1u; i < ranges.size(); i++)
			if (anyEnabled[i]) max = max + ranges[i];

		for (auto i = 1u; i < ranges.size(); i++)
			ranges[i] = ranges[i] + ranges[i - 1].getMax()
			          + (anyEnabled[i - 1] ? max.getMax() / 15 : 0);

		for (auto &bucketIt : subBuckets) {
			int i = 0;
			for (auto &itemIt : bucketIt.second) {
				auto &marker = markers[itemIt.second];
				auto size = marker.getSizeBy(
				    !(bool)options->horizontal.get());

				Base::Align aligner(align, ranges[i]);
				auto newSize = aligner.getAligned(size);

				marker.setSizeBy(!(bool)options->horizontal.get(),
				    newSize);
				i++;
			}
		}
	}
}

void Plot::normalizeSizes()
{
	if (options->shapeType.get() == ShapeType::Circle
	    || options->shapeType.get() == ShapeType::Line) {
		Math::Range<double> size;

		for (auto &marker : markers)
			if (marker.enabled) size.include(marker.sizeFactor);

		auto sizeRange =
		    options->getChannels().at(ChannelId::size).range.get();
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
		color.include(marker.colorBuilder.color);
		lightness.include(marker.colorBuilder.lightness);
	}

	auto colorRange =
	    options->getChannels().at(ChannelId::color).range.get();
	color = colorRange.getRange(color);

	auto lightnessRange =
	    options->getChannels().at(ChannelId::lightness).range.get();
	lightness = lightnessRange.getRange(lightness);

	for (auto &marker : markers) {
		marker.colorBuilder.lightness =
		    lightness.rescale(marker.colorBuilder.lightness);

		if (marker.colorBuilder.continuous())
			marker.colorBuilder.color =
			    color.rescale(marker.colorBuilder.color);

		marker.color = marker.colorBuilder.render();
	}

	stats.channels[ChannelId::color].range = color;
	stats.channels[ChannelId::lightness].range = lightness;

	for (auto &value : dimensionAxises.at(ChannelId::color)) {
		ColorBuilder builder(style.plot.marker.lightnessRange(),
		    *style.plot.marker.colorPalette,
		    (int)value.second.value,
		    0.5);

		value.second.color = builder.render();
	}

	for (auto &value : dimensionAxises.at(ChannelId::lightness)) {
		value.second.value = lightness.rescale(value.second.value);

		ColorBuilder builder(style.plot.marker.lightnessRange(),
		    *style.plot.marker.colorPalette,
		    0,
		    value.second.value);

		value.second.color = builder.render();
	}
}

void Plot::recalcStackedLineChart()
{
	bool isArea = options->shapeType.get() == ShapeType::Area;
	bool isLine = options->shapeType.get() == ShapeType::Line;

	if (options->getChannels().anyAxisSet() && (isArea || isLine)) {
		bool subOnMain = false;
		auto subAxisType = options->stackAxisType();
		for (const auto &series :
		    options->getChannels().at(subAxisType).dimensionIds())
			if (options->mainAxis().isSeriesUsed(series))
				subOnMain = true;

		if (subOnMain) {
			Buckets stackBuckets;
			for (auto i = 0u; i < markers.size(); i++) {
				auto &stackId =
				    isLine ? markers[i].sizeId : markers[i].stackId;
				stackBuckets[stackId.seriesId][stackId.itemId] = i;
			}

			struct Record
			{
				Geom::Point minPoint;
				Geom::Point minSize;
				size_t minIdx;
				Geom::Point maxPoint;
				Geom::Point maxSize;
				size_t maxIdx;
			};
			std::unordered_map<size_t, Record> records;
			const size_t noIdx = -1;

			for (const auto &bucket : stackBuckets) {
				auto &record = records[bucket.first];
				record.minIdx = noIdx;
				record.maxIdx = noIdx;
				for (auto itemId : bucket.second) {
					auto &marker = markers[itemId.second];
					auto nextId =
					    (size_t)marker.nextMainMarkerIdx.values[0]
					        .value;
					auto &nextMarker = markers[nextId];

					if (record.minIdx == noIdx
					    || record.minIdx > itemId.first) {
						record.minIdx = itemId.first;
						record.minPoint = marker.position;
						record.minSize = marker.size;
					}
					if (record.maxIdx == noIdx
					    || record.maxIdx < itemId.first) {
						record.maxIdx = itemId.first;
						record.maxPoint = nextMarker.position;
						record.maxSize = nextMarker.size;
					}
				}
			}

			for (const auto &bucket : stackBuckets) {
				auto &record = records[bucket.first];
				for (auto itemId : bucket.second) {
					auto horizontal = (bool)options->horizontal.get();
					auto &marker = markers[itemId.second];
					auto relpos = marker.position - record.minPoint;
					auto range = record.maxPoint - record.minPoint;
					auto f = (relpos / range).getCoord(horizontal);
					auto intp = Math::interpolate(record.minPoint,
					    record.maxPoint,
					    f);
					marker.position.getCoord(!horizontal) =
					    intp.getCoord(!horizontal);
					auto ints = Math::interpolate(record.minSize,
					    record.maxSize,
					    f);
					marker.size.getCoord(!horizontal) =
					    ints.getCoord(!horizontal);
				}
			}
		}
	}
}

void Plot::prependMarkers(const Plot &plot, bool enabled)
{
	auto size = plot.markers.size();

	markers.insert(markers.begin(),
	    plot.getMarkers().begin(),
	    plot.getMarkers().end());

	if (!enabled)
		for (auto i = 0u; i < size; i++) markers[i].enabled = false;

	for (auto i = size; i < markers.size(); i++)
		markers[i].setIdOffset(size);
}

void Plot::appendMarkers(const Plot &plot, bool enabled)
{
	auto size = markers.size();

	markers.insert(markers.end(),
	    plot.getMarkers().begin(),
	    plot.getMarkers().end());

	for (auto i = size; i < markers.size(); i++) {
		auto &marker = markers[i];

		marker.setIdOffset(size);

		if (!enabled) marker.enabled = false;
	}
}

bool Plot::dimensionMatch(const Plot &a, const Plot &b)
{
	const auto &aDims = a.getOptions()->getChannels().getDimensions();
	const auto &bDims = b.getOptions()->getChannels().getDimensions();
	return (aDims == bDims);
}

}