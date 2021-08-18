#ifndef CHART_GENERATOR_MARKER_H
#define CHART_GENERATOR_MARKER_H

#include "base/anim/interpolated.h"
#include "base/geom/circle.h"
#include "base/geom/point.h"
#include "base/gfx/color.h"
#include "base/math/fuzzybool.h"
#include "chart/main/style.h"
#include "chart/options/options.h"
#include "data/datacube/datacube.h"

#include "colorbuilder.h"

namespace Vizzu
{
namespace Diag
{

class ScalesStats;

class Marker
{
public:
	typedef std::vector<std::pair<std::string, std::string>> CellInfo;

public:

	Marker(const Options &options, const Styles::Chart &style,
				const Data::DataCube &data, const Data::DataTable &table,
				ScalesStats &stats,
				const Data::MultiDim::MultiIndex &index,
				size_t idx);

	Data::MultiDim::MultiIndex index;
	Gfx::Color color;
	ColorBuilder colorBuilder;
	Geom::Point position;
	Geom::Point size;
	Geom::Point spacing;
	double sizeFactor;
	Math::FuzzyBool enabled;
	Math::FuzzyBool selected;
	CellInfo cellInfo;

	struct Label {
		bool hasValue;
		double value;
		std::string unit;
		std::string indexStr;
		Label() : hasValue(false), value(0.0) {}
		Label(const Data::MultiDim::SubSliceIndex &index,
			  const Data::DataCube &data,
			  const Data::DataTable &table);
		Label(double value, const Data::SeriesIndex &continous,
			  const Data::MultiDim::SubSliceIndex &index,
			  const Data::DataCube &data,
			  const Data::DataTable &table);
		bool operator==(const Label& other) const;
		std::string getIndexString(const Data::MultiDim::SubSliceIndex &index,
								   const Data::DataCube &data,
								   const Data::DataTable &table) const;
	};

	::Anim::Interpolated<Label> label;

	struct Id
	{
		uint64_t seriesId;
		Data::MultiDim::SubSliceIndex itemSliceIndex;
		uint64_t itemId;
		Id(){}
		Id(const Data::DataCube &,
		   const Scale::DiscreteIndices &discretesIds,
		   const Data::MultiDim::MultiIndex &);
	};

	Id mainId;
	Id subId;
	Id sizeId;
	Id stackId;

	uint64_t idx;
	::Anim::Interpolated<uint64_t> prevMainMarkerIdx;
	::Anim::Interpolated<uint64_t> nextMainMarkerIdx;
	::Anim::Interpolated<uint64_t> nextSubMarkerIdx;

	void setNextMarker(uint64_t itemId, Marker *marker,
					 bool horizontal, bool main);

	Geom::Rect toRectangle() const;
	void fromRectangle(const Geom::Rect &rect);

	Math::Range<double> getSizeBy(bool horizontal) const;
	void setSizeBy(bool horizontal, const Math::Range<double> range);

	void setIdOffset(size_t offset);
	void fillCellInfo(Data::DataCube& dataCube);

private:
	double
	getValueForScale(const Scales::Level &scales, Scale::Type type,
					 const Data::DataCube &data, ScalesStats &stats,
					 const Scale *subScale = nullptr,
					 bool inhibitStack = false) const;
};

}
}

#endif
