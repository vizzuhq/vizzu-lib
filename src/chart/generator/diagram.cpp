#include "diagram.h"

#include <limits>
#include <numeric>

#include "base/anim/interpolated.h"
#include "base/io/log.h"
#include "base/math/range.h"
#include "chart/speclayout/speclayout.h"
#include "data/datacube/datacube.h"

namespace Vizzu::Diag
{

Diagram::Diagram(DiagramOptionsPtr options, const Diagram &other) :
	options(std::move(options))
{
	anySelected = other.anySelected;
	axises = other.axises;
	discreteAxises = other.discreteAxises;
	anyAxisSet = other.anyAxisSet;
	style = other.style;
	keepAspectRatio = other.keepAspectRatio;
}

Diagram::Diagram(const Data::DataTable &dataTable, DiagramOptionsPtr opts, Styles::Chart style)
	: options(std::move(opts)),
	  style(std::move(style)),
	  dataCube(dataTable, options->getScales().getDataCubeOptions(),
						  options->dataFilters.get(),
						  options->getScales().maxScaleSize()),
	  stats(options->getScales(), dataCube)
{
	anySelected = false;
	anyAxisSet = options->getScales().anyAxisSet();

	generateMarkers(dataCube, dataTable);

	SpecLayout specLayout(*this);
	auto gotSpecLayout = specLayout.addIfNeeded();

	if (gotSpecLayout)
	{
		calcAxises(dataTable);
		calcDiscreteAxises(dataTable);
		normalizeColors();
	}
	else
	{
		addSeparation();
		normalizeXY();
		calcAxises(dataTable);
		calcDiscreteAxises(dataTable);
		addAlignment();
		normalizeSizes();
		recalcStackedLineChart();
		normalizeColors();
	}
}

void Diagram::detachOptions()
{
	options = std::make_shared<Diag::Options>(*options);
}

bool Diagram::isEmpty() const
{
	return options->getScales().isEmpty();
}

void Diagram::generateMarkers(const Data::DataCube &dataCube,
							const Data::DataTable &table)
{
	for (auto it = dataCube.getData().begin();
		 it != dataCube.getData().end(); ++it)
	{
		auto itemIndex = markers.size();

		markers.emplace_back(*options, style, dataCube, table, stats, it.getIndex(),
						   itemIndex);

		auto &marker = markers[itemIndex];

		mainBuckets[marker.mainId.seriesId][marker.mainId.itemId] = itemIndex;
		subBuckets[marker.subId.seriesId][marker.subId.itemId] = itemIndex;
	}
	linkMarkers(mainBuckets, true);
	linkMarkers(subBuckets, false);
}

void Diagram::linkMarkers(const Buckets &buckets, bool main)
{
	size_t maxBucketSize = 0;
	for (const auto &pair : buckets)
		if (pair.second.size() > maxBucketSize)
			maxBucketSize = pair.second.size();

	std::vector<std::pair<uint64_t, double>> sorted;
	sorted.resize(maxBucketSize);
	for (auto &pair: sorted) pair.second = 0;

	for (const auto &pair : buckets)
	{
		const auto &bucket = pair.second;

		for (const auto &id : bucket)
		{
			auto &marker = markers[id.second];
			auto horizontal = (bool)options->horizontal.get();
			auto size = marker.size.getCoord(!horizontal);
			sorted[id.first].first = id.first;
			sorted[id.first].second += size;
		}
	}

	if (main && options->sorted.get())
	{
		std::sort(sorted.begin(), sorted.end(),
		[=](const std::pair<uint64_t, double> &a, const std::pair<uint64_t, double> &b) {
			return a.second < b.second;
		});
	}
	if (main && options->reverse.get())
	{
			std::reverse(sorted.begin(), sorted.end());
	}

	for (const auto &pair : buckets)
	{
		const auto &bucket = pair.second;

		for (auto i = 0u; i < sorted.size(); i++)
		{
			auto idAct = sorted[i].first;
			auto indexAct = bucket.at(idAct);
			auto &act = markers[indexAct];
			auto iNext = (i+1) % sorted.size();
			auto idNext = sorted[iNext].first;
			auto indexNext = bucket.at(idNext);
			act.setNextMarker(iNext, &markers[indexNext],
					(bool)options->horizontal.get() == main, main);
		}
	}
}

void Diagram::normalizeXY()
{
	if (markers.empty()) return;

	auto boundRect = markers.front().toRectangle();

	for (auto &marker: markers)
		boundRect = boundRect.boundary(marker.toRectangle());

	auto xrange = options->getHorizontalAxis().range.get();
	auto yrange = options->getVeritalAxis().range.get();

	boundRect.setHSize(xrange.getValue(boundRect.hSize()));
	boundRect.setVSize(yrange.getValue(boundRect.vSize()));

	for (auto &marker: markers)
		marker.fromRectangle(
			boundRect.normalize(marker.toRectangle())
					);

	stats.scales[Scale::Type::X].range = boundRect.hSize();
	stats.scales[Scale::Type::Y].range = boundRect.vSize();
}

void Diagram::calcAxises(const Data::DataTable &dataTable)
{
	for (auto i = 0u; i < Scale::Type::id_size; i++)
	{
		auto id = Scale::Type(i);
		axises.at(id) = calcAxis(id, dataTable);
	}
}

Axis Diagram::calcAxis(Scale::Type type, const Data::DataTable &dataTable)
{
	const auto *scale = options->getScales().getScales(Scales::Index{0})[type];
	if (!scale->isEmpty() && !scale->isPseudoDiscrete())
	{
		auto title =
			scale->title.get().empty()
			? scale->continousName(dataTable)
			: scale->title.get();

		if (type == options->subAxisType()
			&& options->alignType.get() == Base::Align::Fit)
		{
			return Axis(Math::Range<double>(0,100), title, "%");
		} else {
			auto unit = dataTable.getInfo(scale->continousId()->getColIndex()).getUnit();
			return Axis(stats.scales[type].range, title, unit);
		}
	}
	else return Axis();
}

void Diagram::calcDiscreteAxises(const Data::DataTable &table)
{
	for (auto i = 0u; i < Scale::Type::id_size; i++)
		calcDiscreteAxis(Scale::Type(i), table);
}

void Diagram::calcDiscreteAxis(Scale::Type type,
    const Data::DataTable &table)
{
	auto &axis = discreteAxises.at(type);
	Scales::Id scaleId{type, Scales::Index{0}};
	auto &scale = options->getScales().at(scaleId);
	auto dim = scale.labelLevel.get();

	if (scale.discretesIds().empty() || !scale.isPseudoDiscrete()) return;

	axis.title = scale.title.get();

	if (type == Scale::Type::X || type == Scale::Type::Y)
	{
		for (auto marker : markers)
		{
			auto &id =
			    (type == Scale::Type::X) == options->horizontal.get()
			    ? marker.mainId : marker.subId;

			auto &slice = id.itemSliceIndex;

			if (!slice.empty() && dim >= 0 && dim < slice.size()
			    && dim == floor(dim))
			{
				auto index = slice[dim];
				auto range = marker.getSizeBy(type == Scale::Type::X);
				axis.add(index, id.itemId, range, (double)marker.enabled);
			}
		}
	}
	else
	{
		const auto &indices = stats.scales[type].usedIndices;

		auto count = 0;
		for (auto i = 0u; i < indices.size(); i++)
		{
			const auto &sliceIndex = indices[i];

			if (!sliceIndex.empty() && dim >= 0
			    && dim < sliceIndex.size() && dim == floor(dim))
			{
				auto index = sliceIndex[dim];
				auto range = Math::Range<double>(count, count);
				axis.add(index, i, range, true);
				count++;
			}
		}
	}
	axis.setLabels(dataCube, table);
}

void Diagram::addAlignment()
{
	if ((bool)options->splitted.get()) return;

	auto &axis = axises.at(options->subAxisType());
	if (axis.range.getMin() < 0) return;

	if (options->alignType.get() == Base::Align::None) return;

	for (auto &bucketIt : subBuckets)
	{
		Math::Range<double> range;

		for (auto &itemIt : bucketIt.second)
		{
			auto &marker = markers[itemIt.second];
			auto size = marker.getSizeBy(!(bool)options->horizontal.get());
			range.include(size);
		}

		Base::Align aligner(options->alignType.get(), Math::Range(0.0,1.0));
		auto transform = aligner.getAligned(range) / range;

		for (auto &itemIt : bucketIt.second)
		{
			auto &marker = markers[itemIt.second];
			auto newRange = marker.getSizeBy(!(bool)options->horizontal.get())
							* transform;
			marker.setSizeBy(!(bool)options->horizontal.get(), newRange);
		}
	}
}

void Diagram::addSeparation()
{
	if ((bool)options->splitted.get())
	{
		std::vector<Math::Range<double>> ranges(mainBuckets.size(),
												Math::Range(0.0,0.0));
		std::vector<bool> anyEnabled(mainBuckets.size(), false);

		for (auto &bucketIt : subBuckets)
		{
			auto i = 0u;
			for (auto &itemIt : bucketIt.second)
			{
				auto &marker = markers[itemIt.second];
				auto size = marker.getSizeBy(!(bool)options->horizontal.get()).size();
				ranges[i].include(size);
				if ((double)marker.enabled > 0) anyEnabled[i] = true;
				i++;
			}
		}

		auto max = Math::Range(0.0,0.0);
		for (auto i = 1u; i < ranges.size(); i++)
			if (anyEnabled[i]) max = max + ranges[i];

		for (auto i = 1u; i < ranges.size(); i++)
			ranges[i] = ranges[i] + ranges[i-1].getMax()
						+ (anyEnabled[i-1] ? max.getMax() / 15 : 0);

		for (auto &bucketIt : subBuckets)
		{
			int i = 0;
			for (auto &itemIt : bucketIt.second)
			{
				auto &marker = markers[itemIt.second];
				auto size = marker.getSizeBy(!(bool)options->horizontal.get());

				Base::Align aligner(options->alignType.get(), ranges[i]);
				auto newSize = aligner.getAligned(size);

				marker.setSizeBy(!(bool)options->horizontal.get(), newSize);
				i++;
			}
		}
	}
}

void Diagram::normalizeSizes()
{
	if (options->shapeType.get() == ShapeType::Circle
		|| options->shapeType.get() == ShapeType::Line)
	{
		Math::Range<double> size;

		for (auto &marker : markers) if (marker.enabled)
			size.include(marker.sizeFactor);

		auto sizeRange = options->getScales().at(Scale::Type::Size).range.get();
		size = sizeRange.getValue(size);

		for (auto &marker : markers)
			marker.sizeFactor = size.getMax() == size.getMin()
				? 0 : size.normalize(marker.sizeFactor);
	}
	else
	{
		for (auto &marker: markers) marker.sizeFactor = 0;
	}
}

void Diagram::normalizeColors()
{
	Math::Range<double> lightness;
	Math::Range<double> color;

	for (auto &marker : markers) {
		color.include(marker.colorBuilder.color);
		lightness.include(marker.colorBuilder.lightness);
	}

	auto colorRange = options->getScales().at(Scale::Type::Color).range.get();
	color = colorRange.getValue(color);

	auto lightnessRange = options->getScales().at(Scale::Type::Lightness).range.get();
	lightness = lightnessRange.getValue(lightness);

	for (auto &marker : markers)
	{
		marker.colorBuilder.lightness
			= lightness.rescale(marker.colorBuilder.lightness);

		if (marker.colorBuilder.continous())
			marker.colorBuilder.color
				= color.rescale(marker.colorBuilder.color);

		marker.color = marker.colorBuilder.render();
	}

	for (auto &value : discreteAxises.at(Scale::Type::Color))
	{
		ColorBuilder builder(style.data.lightnessRange(),
		    *style.data.colorPalette, (int)value.second.value, 0.5);

		value.second.color = builder.render();
	}

	for (auto &value : discreteAxises.at(Scale::Type::Lightness))
	{
		value.second.value = lightness.rescale(value.second.value);

		ColorBuilder builder(style.data.lightnessRange(),
		    *style.data.colorPalette, 0, value.second.value);

		value.second.color = builder.render();
	}
}

void Diagram::recalcStackedLineChart()
{
	bool isArea = options->shapeType.get() == ShapeType::Area;
	bool isLine = options->shapeType.get() == ShapeType::Line;

	if (options->getScales().anyAxisSet()
		&& (isArea || isLine) )
	{
		bool subOnMain = false;
		auto subAxisType = options->stackAxisType();
		for (const auto &series : options->getScales().at(subAxisType).discretesIds())
			if (options->mainAxis().isSeriesUsed(series))
				subOnMain = true;

		if (subOnMain)
		{
			Buckets stackBuckets;
			for (auto i = 0u; i < markers.size(); i++)
			{
				auto &stackId = isLine ? markers[i].sizeId : markers[i].stackId;
				stackBuckets[stackId.seriesId][stackId.itemId] = i;
			}

			struct Record {
				Geom::Point minPoint;
				Geom::Point minSize;
				size_t minIdx;
				Geom::Point maxPoint;
				Geom::Point maxSize;
				size_t maxIdx;
			};
			std::unordered_map<size_t, Record> records;
			const size_t noIdx = -1;

			for (const auto &bucket : stackBuckets)
			{
				auto &record = records[bucket.first];
				record.minIdx = noIdx;
				record.maxIdx = noIdx;
				for (auto itemId: bucket.second)
				{
					auto &marker = markers[itemId.second];
					auto nextId = (size_t)marker.nextMainMarkerIdx.values[0].value;
					auto &nextMarker = markers[nextId];

					if (record.minIdx == noIdx || record.minIdx > itemId.first) {
						record.minIdx = itemId.first;
						record.minPoint = marker.position;
						record.minSize = marker.size;
					}
					if (record.maxIdx == noIdx || record.maxIdx < itemId.first) {
						record.maxIdx = itemId.first;
						record.maxPoint = nextMarker.position;
						record.maxSize = nextMarker.size;
					}
				}
			}

			for (const auto &bucket : stackBuckets)
			{
				auto &record = records[bucket.first];
				for (auto itemId: bucket.second)
				{
					auto horizontal = (bool)options->horizontal.get();
					auto &marker = markers[itemId.second];
					auto relpos = marker.position - record.minPoint;
					auto range = record.maxPoint - record.minPoint;
					auto f = (relpos / range).getCoord(horizontal);
					auto intp = Math::interpolate(record.minPoint, record.maxPoint, f);
					marker.position.getCoord(!horizontal) = intp.getCoord(!horizontal);
					auto ints = Math::interpolate(record.minSize, record.maxSize, f);
					marker.size.getCoord(!horizontal) = ints.getCoord(!horizontal);
				}
			}
		}
	}
}

bool Diagram::dimensionMatch(const Diagram &a, const Diagram &b)
{
	const auto &aDims = a.getOptions()->getScales().getDimensions();
	const auto &bDims = b.getOptions()->getScales().getDimensions();
	return (aDims == bDims);
}

}
