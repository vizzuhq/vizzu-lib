#ifndef DIAGRAMOPTIONS_H
#define DIAGRAMOPTIONS_H

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <map>

#include "base/anim/interpolated.h"
#include "base/geom/rect.h"
#include "base/math/fuzzybool.h"
#include "base/math/range.h"
#include "base/refl/enum.h"
#include "base/util/templates.h"
#include "data/datacube/datafilter.h"
#include "data/table/datatable.h"
#include "data/multidim/multidimindex.h"

#include "align.h"
#include "scales.h"
#include "shapetype.h"

namespace Vizzu
{
namespace Diag
{

class Enum(BubbleChartAlgorithm)(slow, experimental);

class Options
{
public:
	static constexpr uint64_t nullMarkerId = (uint64_t)-1;
	typedef ::Anim::Interpolated<std::optional<std::string>> Title;
	typedef ::Anim::Interpolated<std::optional<Scale::Type>> Legend;
	typedef uint64_t MarkerId;
	typedef Data::MultiDim::MultiIndex MarkerIndex;
	typedef std::map<MarkerId, MarkerIndex> MarkersInfoMap;

	Options();

	const Scales &getScales() const { return scales; }
	Scales &getScales() { return scales; }

	void reset();

	Scale::Type mainAxisType() const {
		return horizontal.get() ? Scale::X : Scale::Y;
	}

	Scale::Type subAxisType() const {
		return horizontal.get() ? Scale::Y : Scale::X;
	}

	const Scales::Id mainAxisId(Scales::Index scaleIndex = Scales::Index{0}) const {
		return Scales::Id{ mainAxisType(), scaleIndex };
	}

	const Scales::Id subAxisId(Scales::Index scaleIndex = Scales::Index{0}) const {
		return Scales::Id{ subAxisType(), scaleIndex };
	}

	const Scale &mainAxis(Scales::Index scaleIndex = Scales::Index{0}) const {
		return scales.at(mainAxisId(scaleIndex));
	}

	const Scale &subAxis(Scales::Index scaleIndex = Scales::Index{0}) const {
		return scales.at(subAxisId(scaleIndex));
	}

	const Scale *subAxisOf(Scales::Id id) const;
	Scale::Type stackAxisType() const;

	Util::ReadWrite<Title> title;
	Util::ReadWrite<Math::FuzzyBool> polar;
	Util::ReadWrite<double> angle;
	Util::ReadWrite<ShapeType> shapeType;
	Util::ReadWrite<Math::FuzzyBool> horizontal;
	Util::ReadWrite<Math::FuzzyBool> splitted;
	Util::ReadWrite<Base::Align::Type> alignType;
	Util::ReadWrite<Data::Filter> dataFilter;
	Util::ReadWrite<Math::FuzzyBool> sorted;
	Util::ReadWrite<Math::FuzzyBool> reverse;
	Util::ReadWrite<Legend> legend;
	Util::ReadWrite<MarkersInfoMap> markersInfo;

	Util::ReadWrite<BubbleChartAlgorithm> bubbleChartAlgorithm;

	bool operator==(const Options& other) const;

	Scale::Type getHorizontalScale() const;
	Scale::Type getVeritalScale() const;

	const Scales::Id horizontalAxisId(Scales::Index scaleIndex = Scales::Index{0}) const {
		return Scales::Id{ getHorizontalScale(), scaleIndex };
	}

	const Scales::Id verticalAxisId(Scales::Index scaleIndex = Scales::Index{0}) const {
		return Scales::Id{ getVeritalScale(), scaleIndex };
	}

	const Scale &getHorizontalAxis(Scales::Index scaleIndex = Scales::Index{0}) const {
		return scales.at(horizontalAxisId(scaleIndex));
	}

	const Scale &getVeritalAxis(Scales::Index scaleIndex = Scales::Index{0}) const {
		return scales.at(verticalAxisId(scaleIndex));
	}

	bool isShapeValid(const ShapeType::Type &) const;
	MarkerId markersInfoIdFromIndex(const MarkerIndex &) const;
	MarkerId generateMarkersInfoId() const;

private:
	Scales scales;
	static uint64_t nextMarkersInfoId;
};

typedef std::shared_ptr<Options> DiagramOptionsPtr;

}
}

#endif
