#ifndef SCALE_H
#define SCALE_H

#include <cstdint>
#include <list>
#include <optional>
#include <string>

#include "base/util/templates.h"
#include "data/datacube/datacubeoptions.h"
#include "data/datacube/seriesindex.h"
#include "data/table/datatable.h"

#include "scalerange.h"

namespace Vizzu
{
namespace Diag
{

class Scale : Util::AddReadOnly<Scale>
{
public:

	enum Type : uint32_t { Color, Lightness, Size, Shape, Label, Timeline, X, Y, id_size };

	typedef std::optional<Data::SeriesIndex> OptionalContinousIndex;
	typedef Data::SeriesList DiscreteIndices;

	static Scale makeScale(Type id);

	Scale();
	Scale(const Scale &scale) = default;
	Scale(Type type, double def, bool stackable);
	std::pair<bool, OptionalContinousIndex>
	addSeries(const Data::SeriesIndex &index, std::optional<size_t> pos = std::nullopt);
	bool removeSeries(const Data::SeriesIndex &index);
	bool isSeriesUsed(const Data::SeriesIndex &index) const;
	void reset();
	bool isEmpty() const;
	bool isPseudoDiscrete() const;
	size_t discreteCount() const;
	int findPos(const Data::SeriesIndex &index) const;
	void collectDimesions(Data::DataCubeOptions::IndexSet &dimensions) const;
	void collectRealSeries(Data::DataCubeOptions::IndexSet &series) const;
	std::string continousName(const Data::DataTable &table) const;

	bool operator==(const Scale &other) const;

	ReadOnly<Type> type;
	ReadOnly<OptionalContinousIndex> continousId;
	ReadOnly<DiscreteIndices> discretesIds;
	ReadOnly<double> defaultValue;
	ReadOnly<bool> stackable;
	Util::ReadWrite<ScaleRange> range;
	Util::ReadWrite<double> labelLevel;
	Util::ReadWrite<std::string> title;
};

Scale::DiscreteIndices operator&(const Scale::DiscreteIndices &x,
								 const Scale::DiscreteIndices &y);

std::string toString(Scale::Type type);
Scale::Type toScaleType(const std::string &type);

bool isAxis(Scale::Type type);

}
}

#endif
