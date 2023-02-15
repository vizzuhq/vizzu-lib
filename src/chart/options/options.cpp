
#include "options.h"

#include "base/math/trig.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

static ScaleExtrema operator "" _perc(long double percent)
{
	return ScaleExtrema(percent, ScaleExtremaType::relative);
}

uint64_t Options::nextMarkerInfoId = 1;

Options::Options()
{
	alignType.set(Base::Align::None);
	polar.set(false);
	angle.set(0.0);
	horizontal.set(true);
	sorted.set(false);
	reverse.set(false);
	title.set(std::nullopt);
	tooltipId.set(nullMarkerId);
}

void Options::reset()
{
	scales.reset();
	title.set(title.get().get().has_value() 
		? Title(std::string()) : Title(std::nullopt));
}

const Scale *Options::subAxisOf(ScaleId id) const
{
	switch ((ShapeType::Type)shapeType.get())
	{
	case ShapeType::Type::Rectangle:
		return id == mainAxisType() ? &subAxis() : nullptr;

	case ShapeType::Type::Area:
		return	id == mainAxisType() ? &subAxis() :
				id == subAxisType() ? &mainAxis() :
				nullptr;

	case ShapeType::Type::Line:
		return id == subAxisType()
				|| (id == ScaleId::size && scales.anyAxisSet())
			? &scales.at(ScaleId::size)
			: nullptr;

	case ShapeType::Type::Circle:
		// todo: should return 2 scale (size + other axis)
		if (id == ScaleId::size && scales.anyAxisSet()) {
			return &scales.at(ScaleId::size);
		} else if (isAxis(id)) {
			if (scales.at(id).isPseudoDiscrete() && id == mainAxisType())
				return  &subAxis();
			else
				return &scales.at(ScaleId::size);
		} else return nullptr;

	default:
		return nullptr;
	}
}

ScaleId Options::stackAxisType() const
{
	if (scales.anyAxisSet())
	{
		switch ((ShapeType::Type)shapeType.get())
		{
		case ShapeType::Type::Area:
		case ShapeType::Type::Rectangle: return subAxisType();
		default:
		case ShapeType::Type::Circle:
		case ShapeType::Type::Line: return ScaleId::size;
		}
	}
	else return ScaleId::size;
}

std::optional<ScaleId> Options::secondaryStackType() const
{
	if (scales.anyAxisSet() && shapeType.get() == ShapeType::Line)
		return subAxisType();

	return std::nullopt;
}


Scales Options::shadowScales() const
{
	auto shadow = scales.shadow();

	std::vector<Vizzu::Diag::ScaleId> stackChannels;
	stackChannels.push_back(stackAxisType());
	auto secondary = secondaryStackType(); 
	if (secondary) stackChannels.push_back(*secondary);

	auto stackers = shadow.getDimensions(stackChannels);

	for (auto &stacker : stackers)
	{
		shadow.removeSeries(stackAxisType(), stacker);
		shadow.removeSeries(ScaleId::noop, stacker);
	}
	if (shadow.at(stackAxisType()).continousId()->getType() 
		== Data::SeriesType::Exists)
		shadow.at(stackAxisType()).clearContinuous();

	return shadow;
}

void Options::drilldownTo(const Options& other)
{
	auto &stackAxis = this->stackAxis();

	auto dimensions = other.getScales().getDimensions();

	for (const auto &dim: dimensions)
		if (!getScales().isSeriesUsed(dim)) 
			stackAxis.addSeries(dim);
}

void Options::intersection(const Options& other)
{
	auto dimensions = getScales().getDimensions();

	for (const auto &dim: dimensions)
		if (!other.getScales().isSeriesUsed(dim))
	{
		getScales().removeSeries(dim);
	}
}

bool Options::looksTheSame(const Options& other) const
{
	if (scales.anyAxisSet() && scales.at(Diag::ScaleId::label).isEmpty())
	{
		auto thisCopy = *this;
		thisCopy.simplify();

		auto otherCopy = other;
		otherCopy.simplify();

		return thisCopy == otherCopy;
	}
	else return *this == other;
}

void Options::simplify()
{
//	remove all dimensions, only used at the end of stack
	auto &stackAxis = this->stackAxis();

	auto dimensions = stackAxis.discretesIds();

	auto copy = getScales();
	copy.at(stackAxisType()).reset();

	auto dim = dimensions.rbegin();
	for (; dim != dimensions.rend(); ++dim)
	{
		if (!copy.isSeriesUsed(*dim)) 
			stackAxis.removeSeries(*dim);
		else break;
	}
}

bool Options::operator==(const Options &other) const
{
	return scales == other.scales && sameAttributes(other);
}

bool Options::sameShadow(const Options& other) const
{
	return shadowScales() == other.shadowScales()
		&& sameShadowAttribs(other);
}

bool Options::sameShadowAttribs(const Options& other) const
{
	auto shape = shapeType.get();
	if (shape == ShapeType::Line) shape = ShapeType::Area;

	auto shapeOther = other.shapeType.get();
	if (shapeOther == ShapeType::Line) shapeOther = ShapeType::Area;

	return shape == shapeOther
	        && polar.get() == other.polar.get()
	        && angle.get() == other.angle.get()
	        && horizontal.get() == other.horizontal.get()
	        && splitted.get() == other.splitted.get()
	        && dataFilter.get() == other.dataFilter.get()
	        && alignType.get() == other.alignType.get()
	        && splitted.get() == other.splitted.get()
	        && sorted.get() == other.sorted.get()
	        && reverse.get() == other.reverse.get();
}

bool Options::sameAttributes(const Options& other) const
{
	return sameShadowAttribs(other)
	        && shapeType.get() == other.shapeType.get()
			&& title.get() == other.title.get()
			&& legend.get() == other.legend.get()
			&& markersInfo.get() == other.markersInfo.get();
}

ScaleId Options::getHorizontalScale() const
{
	return (Math::rad2quadrant(angle.get()) % 2) == 0
			? ScaleId::x : ScaleId::y;
}

ScaleId Options::getVerticalScale() const
{
	return getHorizontalScale() == ScaleId::x
			? ScaleId::y : ScaleId::x;
}

bool Options::isShapeValid(const ShapeType::Type &shapeType) const
{
	if (scales.anyAxisSet() && mainAxis().discreteCount() > 0)
		return true;
	else
		return shapeType == ShapeType::Rectangle || shapeType == ShapeType::Circle;
}

uint64_t Options::getMarkerInfoId(MarkerId id) const {
	for(auto& i : markersInfo.get()) {
		if (i.second == id)
			return i.first;
	}
	return nullMarkerInfoId;
}

uint64_t Options::generateMarkerInfoId() const {
	return nextMarkerInfoId++;
}

void Options::setAutoParameters()
{
	if (legend.get().get().isAuto()) 
	{
		Base::AutoParam<ScaleId> tmp = legend.get().get();
		tmp.setAuto(getAutoLegend());
		legend.set(tmp);
	}
}

std::optional<ScaleId> Options::getAutoLegend()
{
	auto series = scales.getDimensions();
	series.merge(scales.getSeries());

	for (auto id: scales.at(ScaleId::label).discretesIds())
		series.erase(id);

	if (!scales.at(ScaleId::label).isPseudoDiscrete())
		series.erase(*scales.at(ScaleId::label).continousId());

	for (auto scaleId : { ScaleId::x, ScaleId::y })
	{
		auto id = scales.at(scaleId).labelSeries();
		if (id) series.erase(*id);
	}

	for (auto scaleId : { ScaleId::color, ScaleId::lightness })
		for (auto id: scales.at(scaleId).discretesIds())
			if (series.contains(id))
				return scaleId;

	for (auto scaleId : { ScaleId::color, ScaleId::lightness, ScaleId::size })
		if (!scales.at(scaleId).isPseudoDiscrete())
			if (series.contains(*scales.at(scaleId).continousId()))
				return scaleId;

	return std::nullopt;
}

void Options::setAutoRange(bool hPositive, bool vPositive)
{
	auto &v = getVeritalAxis();
	auto &h = getHorizontalAxis();

	if (!scales.anyAxisSet())
	{
		setRange(h, 0.0_perc, 100.0_perc);
		setRange(v, 0.0_perc, 100.0_perc);
	}
	else if (!(bool)polar.get())
	{
		if (!h.isPseudoDiscrete() 
			&& !v.isPseudoDiscrete()
			&& shapeType.get() == ShapeType::Rectangle)
		{
			setRange(h, 0.0_perc, 100.0_perc);
			setRange(v, 0.0_perc, 100.0_perc);
		}
		else
		{
			if (h.isPseudoDiscrete()) setRange(h, 0.0_perc, 100.0_perc);
			else setContinousRange(h, hPositive); 

			if (v.isPseudoDiscrete()) setRange(v, 0.0_perc, 100.0_perc);
			else setContinousRange(v, vPositive); 
		}
	}
	else 
	{
		if (!h.isPseudoDiscrete() && v.isPseudoDiscrete())
		{
			if (v.isEmpty())
			{
				setRange(h, 0.0_perc, 100.0_perc);
				setRange(v, 0.0_perc, 100.0_perc);
			}
			else
			{
				setRange(h, 0.0_perc, 133.0_perc);
				setRange(v, 0.0_perc, 100.0_perc);
			}
		}
		else if (h.isPseudoDiscrete() && !v.isPseudoDiscrete())
		{
			setRange(h, 0.0_perc, 100.0_perc);
			setContinousRange(v, vPositive);
		}
		else
		{
			setRange(h, 0.0_perc, 100.0_perc);
			setRange(v, 0.0_perc, 100.0_perc);
		}
	}
}

void Options::setContinousRange(Scale &scale, bool positive)
{
	if (positive) setRange(scale, 0.0_perc, 110.0_perc); 
	else setRange(scale, ScaleExtrema(-10, ScaleExtremaType::relative), 110.0_perc);
}

void Options::setRange(Scale &scale, ScaleExtrema min, ScaleExtrema max)
{
	if (scale.range.ref().max.isAuto())
		scale.range.ref().max.setAuto(max);

	if (scale.range.ref().min.isAuto())
		scale.range.ref().min.setAuto(min);
}
