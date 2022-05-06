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
#include "data2/datacube/datafilter.h"
#include "data2/table/datatable.h"
#include "data2/multidim/multidimindex.h"

#include "align.h"
#include "scales.h"
#include "shapetype.h"
#include "autoparam.h"

namespace Vizzu
{
namespace Diag
{

class Options
{
public:
	typedef uint64_t MarkerId;
	static constexpr MarkerId nullMarkerId = (uint64_t)-1;
	static constexpr uint64_t nullMarkerInfoId = (uint64_t)-1;
	typedef ::Anim::Interpolated<std::optional<std::string>> Title;
	typedef ::Anim::Interpolated<Base::AutoParam<ScaleId>> Legend;
	typedef std::map<uint64_t, MarkerId> MarkersInfoMap;

	Options();

	const Scales &getScales() const { return scales; }
	Scales &getScales() { return scales; }

	void reset();

	ScaleId mainAxisType() const {
		return horizontal.get() ? ScaleId::x : ScaleId::y;
	}

	ScaleId subAxisType() const {
		return horizontal.get() ? ScaleId::y : ScaleId::x;
	}

	const Scale &mainAxis() const {
		return scales.at(mainAxisType());
	}

	const Scale &subAxis() const {
		return scales.at(subAxisType());
	}

	Scale &mainAxis() {
		return scales.at(mainAxisType());
	}

	Scale &subAxis() {
		return scales.at(subAxisType());
	}

	const Scale *subAxisOf(ScaleId id) const;
	ScaleId stackAxisType() const;

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
	Util::ReadWrite<uint64_t> tooltipId;
	Util::ReadWrite<MarkersInfoMap> markersInfo;

	bool operator==(const Options& other) const;

	ScaleId getHorizontalScale() const;
	ScaleId getVerticalScale() const;

	const Scale &getHorizontalAxis() const {
		return scales.at(getHorizontalScale());
	}

	const Scale &getVeritalAxis() const {
		return scales.at(getVerticalScale());
	}

	Scale &getHorizontalAxis() {
		return scales.at(getHorizontalScale());
	}

	Scale &getVeritalAxis() {
		return scales.at(getVerticalScale());
	}

	bool isShapeValid(const ShapeType::Type &) const;
	uint64_t getMarkerInfoId(MarkerId) const;
	uint64_t generateMarkerInfoId() const;

	void setAutoParameters();
	void setAutoRange(bool hPositive, bool vPositive);

private:
	Scales scales;

	std::optional<ScaleId> getAutoLegend();
	void setContinousRange(Scale &scale, bool positive);
	void setRange(Scale &scale, ScaleExtrema min, ScaleExtrema max);
	static uint64_t nextMarkerInfoId;
};

typedef std::shared_ptr<Options> DiagramOptionsPtr;

}
}

#endif
