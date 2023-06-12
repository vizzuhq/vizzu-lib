#include "planner.h"

#include "base/anim/easingfunc.h"
#include "base/io/log.h"

#include "morph.h"
#include "styles.h"

using namespace Vizzu;
using namespace Vizzu::Anim;
using namespace std::literals::chrono_literals;

void Planner::createPlan(const Diag::Diagram &source,
    const Diag::Diagram &target,
    Diag::Diagram &actual,
    const Options::Keyframe &options)
{
	this->source = &source;
	this->target = &target;
	this->actual = &actual;
	this->options = &options;

	const auto &srcOpt = source.getOptions();
	const auto &trgOpt = target.getOptions();
	const auto &actOpt = actual.getOptions();

	reset();
	calcNeeded();

	::Anim::Duration baseStep(1125ms);
	::Anim::Duration step(baseStep);

	if (Diag::Diagram::dimensionMatch(source, target)) {
		addMorph(SectionId::hide, baseStep);

		setBaseline();

		::Anim::Duration delay;
		::Anim::Duration xdelay;
		::Anim::Duration posDuration;

		::Anim::Easing in3(
		    &::Anim::EaseFunc::in<&::Anim::EaseFunc::cubic>);
		::Anim::Easing out3(
		    &::Anim::EaseFunc::out<&::Anim::EaseFunc::cubic>);
		::Anim::Easing inOut3(
		    &::Anim::EaseFunc::inOut<&::Anim::EaseFunc::cubic>);
		::Anim::Easing inOut5(
		    &::Anim::EaseFunc::inOut<&::Anim::EaseFunc::quint>);

		if (positionMorphNeeded()) {
			if (animNeeded[SectionId::x] && animNeeded[SectionId::y])
				step = 1600ms;

			addMorph(SectionId::x, step);
			addMorph(SectionId::y, step);
			posDuration += step;
		}
		else {
			auto first = verticalBeforeHorizontal() ? SectionId::y
			                                        : SectionId::x;
			auto second =
			    first == SectionId::y ? SectionId::x : SectionId::y;

			if (animNeeded[first]) {
				addMorph(first,
				    step,
				    0ms,
				    animNeeded[second] ? in3 : inOut3);
				delay = step;
				posDuration += step;
			}

			if (animNeeded[second]) {
				addMorph(second,
				    step,
				    delay,
				    animNeeded[first] ? out3 : inOut3);
				if (second == SectionId::x) xdelay = delay;
				posDuration += step;
			}
		}

		if (animNeeded[SectionId::style])
			Morph::StyleMorphFactory(source.getStyle(),
			    target.getStyle(),
			    actual.getStyle())
			    .populate(*this, getOptions(SectionId::style, step));

		if (animNeeded[SectionId::legend])
			addElement(
			    std::make_unique<
			        ::Anim::SingleElement<Diag::Options::Legend>>(
			        srcOpt->legend.ref(),
			        trgOpt->legend.ref(),
			        actOpt->legend.ref()),
			    getOptions(SectionId::legend, step));

		addMorph(SectionId::color, step);
		addMorph(SectionId::coordSystem, std::max(step, posDuration));

		auto &geomEasing =
		    srcOpt->shapeType.get() == Diag::ShapeType::Circle ? in3
		    : trgOpt->shapeType.get() == Diag::ShapeType::Circle
		        ? out3
		    : srcOpt->shapeType.get() == Diag::ShapeType::Line ? in3
		    : trgOpt->shapeType.get() == Diag::ShapeType::Line
		        ? out3
		        : inOut5;

		addMorph(SectionId::geometry,
		    std::max(step, posDuration),
		    0ms,
		    geomEasing);

		setBaseline();

		addMorph(SectionId::show, baseStep);
	}
	else {
		if (animNeeded[SectionId::show]
		    && animNeeded[SectionId::hide]) {
			::Anim::Easing in3(
			    &::Anim::EaseFunc::in<&::Anim::EaseFunc::cubic>);

			addMorph(SectionId::show, step);
			addMorph(SectionId::hide, step, 0ms, in3);
		}
		else if (animNeeded[SectionId::show])
			addMorph(SectionId::show, step);
		else if (animNeeded[SectionId::hide])
			addMorph(SectionId::hide, step);

		resetBaseline();

		addMorph(SectionId::x, step);

		addMorph(SectionId::y, step);

		if (animNeeded[SectionId::style])
			Morph::StyleMorphFactory(source.getStyle(),
			    target.getStyle(),
			    actual.getStyle())
			    .populate(*this, getOptions(SectionId::style, step));

		if (animNeeded[SectionId::legend])
			addElement(
			    std::make_unique<
			        ::Anim::SingleElement<Diag::Options::Legend>>(
			        srcOpt->legend.ref(),
			        trgOpt->legend.ref(),
			        actOpt->legend.ref()),
			    getOptions(SectionId::legend, step));

		addMorph(SectionId::color, step);
		addMorph(SectionId::coordSystem, step);
		addMorph(SectionId::geometry, step);
	}

	resetBaseline();

	if (animNeeded[SectionId::title]) {
		::Anim::Easing easing(
		    &::Anim::EaseFunc::middle<&::Anim::EaseFunc::quint>);

		auto duration =
		    (double)this->duration > 0 ? this->duration : 1s;

		addElement(std::make_unique<
		               ::Anim::SingleElement<Diag::Options::Title>>(
		               srcOpt->title.ref(),
		               trgOpt->title.ref(),
		               actOpt->title.ref()),
		    getOptions(SectionId::title, duration, 0s, easing));
	}

	if (animNeeded[SectionId::tooltip]) {
		addElement(
		    std::make_unique<
		        ::Anim::SingleElement<Diag::Diagram::MarkersInfo>>(
		        source.getMarkersInfo(),
		        target.getMarkersInfo(),
		        actual.getMarkersInfo()),
		    getOptions(SectionId::tooltip, 300ms));
	}

	reTime();
}

void Planner::reTime()
{
	if (options->all.duration && options->all.delay)
		::Anim::Group::reTime(*options->all.duration,
		    *options->all.delay);
	else if (options->all.duration)
		::Anim::Group::reTime(*options->all.duration, 0s);
	else if (options->all.delay && (double)this->duration != 0.0)
		::Anim::Group::reTime(this->duration, *options->all.delay);
}

void Planner::reset()
{
	::Anim::Group::clear();

	for (auto i = 0u; i < SectionId::EnumInfo::count(); i++)
		animNeeded[i] = false;
}

void Planner::calcNeeded()
{
	const auto &srcOpt = source->getOptions();
	const auto &trgOpt = target->getOptions();

	animNeeded[SectionId::style] =
	    Morph::StyleMorphFactory(source->getStyle(),
	        target->getStyle(),
	        actual->getStyle())
	        .isNeeded();

	animNeeded[SectionId::title] =
	    srcOpt->title.get() != trgOpt->title.get();
	animNeeded[SectionId::tooltip] =
	    srcOpt->markersInfo.get() != trgOpt->markersInfo.get();

	animNeeded[SectionId::legend] =
	    ((bool)srcOpt->legend.get().get()
	        != (bool)trgOpt->legend.get().get())
	    || ((bool)srcOpt->legend.get().get()
	        && (bool)trgOpt->legend.get().get()
	        && (*srcOpt->legend.get().get()
	            != *trgOpt->legend.get().get()));

	animNeeded[SectionId::show] = anyMarker(
	    [&](const auto &source, const auto &target)
	    {
		    return (bool)(!source.enabled && target.enabled);
	    });

	animNeeded[SectionId::hide] = anyMarker(
	    [&](const auto &source, const auto &target) -> bool
	    {
		    return (bool)(source.enabled && !target.enabled);
	    });

	animNeeded[SectionId::color] = needColor();

	animNeeded[SectionId::coordSystem] =
	    srcOpt->polar.get() != trgOpt->polar.get()
	    || srcOpt->angle.get() != trgOpt->angle.get();

	animNeeded[SectionId::geometry] =
	    srcOpt->shapeType.get() != trgOpt->shapeType.get();

	animNeeded[SectionId::y] = needVertical();
	animNeeded[SectionId::x] = needHorizontal();
}

bool Planner::anyMarker(const std::function<bool(const Diag::Marker &,
        const Diag::Marker &)> &compare) const
{
	for (auto i = 0u; i < source->getMarkers().size()
	                  && i < target->getMarkers().size();
	     i++) {
		if (compare(source->getMarkers()[i], target->getMarkers()[i]))
			return true;
	}
	return false;
}

bool Planner::positionMorphNeeded() const
{
	typedef Diag::ShapeType ST;

	auto &srcShape = source->getOptions()->shapeType.get();
	auto &trgShape = target->getOptions()->shapeType.get();

	auto anyCircle = srcShape == ST::Circle || trgShape == ST::Circle;

	if (anyCircle) return true;

	auto anyRectangle =
	    srcShape == ST::Rectangle || trgShape == ST::Rectangle;

	if (anyRectangle) return false;

	return true;
}

bool Planner::needColor() const
{
	return source->anySelected != target->anySelected
	    || (isAnyLegend(Diag::ScaleId::color)
	        && (source->discreteAxises.at(Diag::ScaleId::color)
	                != target->discreteAxises.at(Diag::ScaleId::color)
	            || source->axises.at(Diag::ScaleId::color)
	                   != target->axises.at(Diag::ScaleId::color)))
	    || (isAnyLegend(Diag::ScaleId::lightness)
	        && (source->discreteAxises.at(Diag::ScaleId::lightness)
	                != target->discreteAxises.at(
	                    Diag::ScaleId::lightness)
	            || source->axises.at(Diag::ScaleId::lightness)
	                   != target->axises.at(
	                       Diag::ScaleId::lightness)))
	    || anyMarker(
	        [&](const auto &source, const auto &target)
	        {
		        return (source.enabled || target.enabled)
		            && (source.color != target.color
		                || source.selected != target.selected);
	        });
}

size_t Planner::discreteCount(const Diag::Diagram *diagram,
    Diag::ScaleId type) const
{
	return diagram->getOptions()
	    ->getScales()
	    .at(type)
	    .discretesIds()
	    .size();
}

bool Planner::verticalBeforeHorizontal() const
{
	const auto &srcOpt = source->getOptions();
	const auto &trgOpt = target->getOptions();

	if (srcOpt->horizontal.get() != trgOpt->horizontal.get()
	    || !srcOpt->getScales().anyAxisSet()
	    || !trgOpt->getScales().anyAxisSet()) {
		if (srcOpt->getScales().anyAxisSet())
			return srcOpt->subAxisType() == Diag::ScaleId::y;
		else if (trgOpt->getScales().anyAxisSet())
			return trgOpt->mainAxisType() == Diag::ScaleId::y;
	}

	auto srcXcnt = discreteCount(source, Diag::ScaleId::x);
	auto srcYcnt = discreteCount(source, Diag::ScaleId::y);
	auto trgXcnt = discreteCount(target, Diag::ScaleId::x);
	auto trgYcnt = discreteCount(target, Diag::ScaleId::y);

	if ((trgYcnt != srcYcnt) || (trgXcnt != srcXcnt)) {
		return (trgYcnt > srcYcnt) || (trgXcnt < srcXcnt);
	}
	else {
		return !(bool)source->getOptions()->horizontal.get();
	}
}

bool Planner::needVertical() const
{
	return source->axises.at(Diag::ScaleId::y)
	        != target->axises.at(Diag::ScaleId::y)
	    || source->discreteAxises.at(Diag::ScaleId::y)
	           != target->discreteAxises.at(Diag::ScaleId::y)
	    || source->guides.at(Diag::ScaleId::y)
	           != target->guides.at(Diag::ScaleId::y)
	    || (isAnyLegend(Diag::ScaleId::size)
	        && (source->axises.at(Diag::ScaleId::size)
	                != target->axises.at(Diag::ScaleId::size)
	            || source->discreteAxises.at(Diag::ScaleId::size)
	                   != target->discreteAxises.at(
	                       Diag::ScaleId::size)))
	    || source->anyAxisSet != target->anyAxisSet
	    || anyMarker(
	        [&](const auto &source, const auto &target)
	        {
		        return (source.enabled || target.enabled)
		            && (source.position.y != target.position.y
		                || source.spacing.y != target.spacing.y
		                || source.size.y != target.size.y
		                || source.sizeFactor != target.sizeFactor
		                || source.label != target.label);
	        });
}

bool Planner::needHorizontal() const
{
	return source->axises.at(Diag::ScaleId::x)
	        != target->axises.at(Diag::ScaleId::x)
	    || source->discreteAxises.at(Diag::ScaleId::x)
	           != target->discreteAxises.at(Diag::ScaleId::x)
	    || source->guides.at(Diag::ScaleId::x)
	           != target->guides.at(Diag::ScaleId::x)
	    || source->anyAxisSet != target->anyAxisSet
	    || source->keepAspectRatio != target->keepAspectRatio
	    || anyMarker(
	        [&](const auto &source, const auto &target)
	        {
		        return (source.enabled || target.enabled)
		            && (source.position.x != target.position.x
		                || source.spacing.x != target.spacing.x
		                || source.size.x != target.size.x);
	        });
}

bool Planner::isAnyLegend(Diag::ScaleId type) const
{
	const auto &src = source->getOptions()->legend.get().get();
	const auto &trg = target->getOptions()->legend.get().get();
	return (src && *src == type) || (trg && *trg == type);
}

void Planner::addMorph(SectionId sectionId,
    ::Anim::Duration duration,
    ::Anim::Duration delay,
    std::optional<::Anim::Easing> easing)
{
	if (animNeeded[sectionId]) {
		addElement(Morph::AbstractMorph::create(sectionId,
		               *source,
		               *target,
		               *actual),
		    getOptions(sectionId, duration, delay, easing));
	}
}

::Anim::Options Planner::getOptions(SectionId sectionId,
    ::Anim::Duration duration,
    ::Anim::Duration delay,
    std::optional<::Anim::Easing> easing)
{
	const auto &opt = options->get(sectionId);
	if (opt.duration) duration = *opt.duration;
	if (opt.delay) delay = *opt.delay - getBaseline();
	return ::Anim::Options(duration,
	    delay,
	    getEasing(sectionId, easing));
}

::Anim::Easing Planner::getEasing(SectionId type,
    const std::optional<::Anim::Easing> &def) const
{
	auto res = def ? *def : defEasing();
	if (options->all.easing) res = *options->all.easing;
	if (options->get(type).easing) res = *options->get(type).easing;
	return res;
}

::Anim::Easing Planner::defEasing() const
{
	return ::Anim::Easing(
	    &::Anim::EaseFunc::inOut<&::Anim::EaseFunc::cubic>);
}
