#ifndef DIAGRAMOPTIONS_H
#define DIAGRAMOPTIONS_H

#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "base/anim/interpolated.h"
#include "base/geom/rect.h"
#include "base/math/fuzzybool.h"
#include "base/math/range.h"
#include "base/refl/enum.h"
#include "base/util/templates.h"
#include "data/datacube/datafilter.h"
#include "data/table/datatable.h"

#include "align.h"
#include "scales.h"
#include "shapetype.h"
#include "autoparam.h"

namespace Vizzu
{
namespace Diag
{

class Enum(BubbleChartAlgorithm)(slow, experimental);

class Options
{
public:
	typedef ::Anim::Interpolated<std::optional<std::string>> Title;
	typedef ::Anim::Interpolated<Base::AutoParam<ScaleId>> Legend;

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

	Util::ReadWrite<BubbleChartAlgorithm> bubbleChartAlgorithm;

	bool operator==(const Options& other) const;

	ScaleId getHorizontalScale() const;
	ScaleId getVeritalScale() const;

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

private:

	Scales scales;
};

typedef std::shared_ptr<Options> DiagramOptionsPtr;

}
}

#endif
