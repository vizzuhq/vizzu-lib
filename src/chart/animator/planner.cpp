#include "planner.h"

#include "base/io/log.h"
#include "base/anim/easingfunc.h"

#include "morph.h"
#include "styles.h"

using namespace Vizzu;
using namespace Vizzu::Anim;
using namespace std::literals::chrono_literals;

void Planner::createPlan(const Diag::Diagram &source,
    const Diag::Diagram &target,
    Diag::Diagram &actual,
    const Options &options)
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

	::Anim::Duration step(1125ms);

	if(Diag::Diagram::dimensionMatch(source, target))
	{
		addMorph(SectionId::hide, step);

		setBaseline();

		::Anim::Duration delay;
		::Anim::Duration xdelay;

		if (positionMorphNeeded())
		{
			if (animNeeded[SectionId::x] && animNeeded[SectionId::y])
				step = 1600ms;

			addMorph(SectionId::x, step);
			addMorph(SectionId::y, step);
		}
		else
		{
			::Anim::Easing in(&::Anim::EaseFunc::in<&::Anim::EaseFunc::cubic>);
			::Anim::Easing out(&::Anim::EaseFunc::out<&::Anim::EaseFunc::cubic>);

			auto first = verticalBeforeHorizontal() ?  SectionId::y : SectionId::x;
			auto second = first == SectionId::y ? SectionId::x : SectionId::y;

			if (animNeeded[first]) {
				addMorph(first, step, 0ms, in);
				delay = step;
			}

			if (animNeeded[second]) {
				addMorph(second, step, delay, out);
				if (second == SectionId::x) xdelay = delay;
			}
		}

		if (animNeeded[SectionId::style])
			Morph::StyleMorphFactory(
				source.getStyle(), target.getStyle(), actual.getStyle())
			.populate(*this, ::Anim::Options(step, 0s, 
				getEasing(SectionId::style)));

		if (animNeeded[SectionId::legend])
			addElement(
				std::make_unique<::Anim::SingleElement<Diag::Options::Legend>>(
					srcOpt->legend.ref(), trgOpt->legend.ref(), actOpt->legend.ref()),
				::Anim::Options(step, 0s, getEasing(SectionId::legend))
			);

		addMorph(SectionId::color, step);
		addMorph(SectionId::coordSystem, step, xdelay);

		auto geomDelay = 
			(bool)srcOpt->shapeType.get().getFactor(Diag::ShapeType::Circle)
			? 0s : delay;

		addMorph(SectionId::geometry, step, geomDelay);

		setBaseline();

		addMorph(SectionId::show, step);
	}
	else
	{
		if (animNeeded[SectionId::show] && animNeeded[SectionId::hide])
		{
			::Anim::Easing in(&::Anim::EaseFunc::in<&::Anim::EaseFunc::cubic>);

			addMorph(SectionId::show, step);
			addMorph(SectionId::hide, step, 0ms, in);
		}
		else if (animNeeded[SectionId::show]) addMorph(SectionId::show, step);
		else if (animNeeded[SectionId::hide]) addMorph(SectionId::hide, step);

		resetBaseline();

		addMorph(SectionId::x, step);

		addMorph(SectionId::y, step);

		if (animNeeded[SectionId::style])
			Morph::StyleMorphFactory(
				source.getStyle(), target.getStyle(), actual.getStyle())
			.populate(*this, ::Anim::Options(step, 0s, 
				getEasing(SectionId::style)));

		if (animNeeded[SectionId::legend])
			addElement(
				std::make_unique<::Anim::SingleElement<Diag::Options::Legend>>(
					srcOpt->legend.ref(), trgOpt->legend.ref(), actOpt->legend.ref()),
				::Anim::Options(step, 0s, getEasing(SectionId::legend))
			);

		addMorph(SectionId::color, step);
		addMorph(SectionId::coordSystem, step);
		addMorph(SectionId::geometry, step);
	}

	resetBaseline();

	if (animNeeded[SectionId::title])
	{
		::Anim::Easing easing(&::Anim::EaseFunc::in<&::Anim::EaseFunc::cubic>);

		auto duration = (double)this->duration > 0 ? this->duration : 1s;

		addElement(
			std::make_unique<::Anim::SingleElement<Diag::Options::Title>>(
				srcOpt->title.ref(), trgOpt->title.ref(), actOpt->title.ref()),
			::Anim::Options(duration, 0s, getEasing(SectionId::color, easing))
		);
	}

	reTime();
}

void Planner::reTime()
{
	if (options->all.duration && options->all.delay)
		::Anim::Group::reTime(*options->all.duration, *options->all.delay);
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
		Morph::StyleMorphFactory(source->getStyle(), target->getStyle(),
	    	actual->getStyle()).isNeeded();

	animNeeded[SectionId::title] = srcOpt->title.get() != trgOpt->title.get();
	animNeeded[SectionId::legend] = srcOpt->legend.get() != trgOpt->legend.get();
	
	animNeeded[SectionId::show] = anyMarker(
		[&](const auto &source, const auto &target) {
			return (bool)(!source.enabled && target.enabled);
		});
	
	animNeeded[SectionId::hide] = anyMarker(
		[&](const auto &source, const auto &target) -> bool {
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

bool Planner::anyMarker(
	const std::function<
		bool(const Diag::Marker &, const Diag::Marker &)
	> &compare) const
{
	for (auto i = 0u;
	     i < source->getMarkers().size() && i < target->getMarkers().size();
	     i++)
	{
		if (compare(source->getMarkers()[i], target->getMarkers()[i]))
			return true;
	}
	return false;
}

bool Planner::positionMorphNeeded() const
{
	return Diag::canOverlap(
	           (Diag::ShapeType::Type)source->getOptions()->shapeType.get())
	    || Diag::canOverlap(
	        (Diag::ShapeType::Type)target->getOptions()->shapeType.get());
}

bool Planner::needColor() const
{
	return source->anySelected != target->anySelected
		|| 
		(
			isAnyLegend(Diag::ScaleId::color)
			&&
			(
				source->discreteAxises.at(Diag::ScaleId::color)
					!= target->discreteAxises.at(Diag::ScaleId::color)
				|| source->axises.at(Diag::ScaleId::color)
					!= target->axises.at(Diag::ScaleId::color)
			)
		)
		||
		(
			isAnyLegend(Diag::ScaleId::lightness)
			&&
			(
				source->discreteAxises.at(Diag::ScaleId::lightness)
					!= target->discreteAxises.at(Diag::ScaleId::lightness)
				|| source->axises.at(Diag::ScaleId::lightness)
					!= target->axises.at(Diag::ScaleId::lightness)
			)
		)
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
	return diagram->getOptions()->getScales()
		.at(Diag::Scales::Id{type, Diag::Scales::Index{0}})
		.discretesIds().size();
}

bool Planner::verticalBeforeHorizontal() const
{
	const auto &srcOpt = source->getOptions();
	const auto &trgOpt = target->getOptions();

	if (srcOpt->horizontal.get() != trgOpt->horizontal.get())
	{
		if (srcOpt->getScales().anyAxisSet())
			return srcOpt->subAxisType() == Diag::ScaleId::y;
		else if (trgOpt->getScales().anyAxisSet())
			return trgOpt->subAxisType() == Diag::ScaleId::y;
	}

	auto srcXcnt = discreteCount(source, Diag::ScaleId::x);
	auto srcYcnt = discreteCount(source, Diag::ScaleId::y);
	auto trgXcnt = discreteCount(target, Diag::ScaleId::x);
	auto trgYcnt = discreteCount(target, Diag::ScaleId::y);

	if ((trgYcnt != srcYcnt) || (trgXcnt != srcXcnt))
	{
		return (trgYcnt > srcYcnt) || (trgXcnt < srcXcnt);
	}
	else
	{
		auto sourceHor = (bool)source->getOptions()->horizontal.get();
		auto targetHor = (bool)target->getOptions()->horizontal.get();

		return sourceHor == targetHor ? !sourceHor : sourceHor;
	}
}

bool Planner::needVertical() const
{
	return source->axises.at(Diag::ScaleId::y)
			!= target->axises.at(Diag::ScaleId::y)
		|| source->discreteAxises.at(Diag::ScaleId::y)
			!= target->discreteAxises.at(Diag::ScaleId::y)
		|| 
		(
			isAnyLegend(Diag::ScaleId::size)
			&&
			(
				source->axises.at(Diag::ScaleId::size)
					!= target->axises.at(Diag::ScaleId::size)
				|| source->discreteAxises.at(Diag::ScaleId::size)
					!= target->discreteAxises.at(Diag::ScaleId::size)
			)
		)
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
	return (src && *src == type)
		|| (trg && *trg == type);
}

void Planner::addMorph(
	SectionId sectionId, 
	::Anim::Duration duration, 
	::Anim::Duration delay, 
	std::optional<::Anim::Easing> easing)
{
	if (animNeeded[sectionId])
		addElement(
			Morph::AbstractMorph::create(sectionId, *source, *target, *actual),
			::Anim::Options(duration, delay, getEasing(sectionId, easing))
		);
}

::Anim::Easing Planner::getEasing(SectionId type, 
	const std::optional<::Anim::Easing> &def) const
{
	auto res = def ? *def : defEasing();
	if (options->all.easing) res = *options->all.easing;
	if (options->get(type).easing) res = *options->all.easing;
	return res;
}

::Anim::Easing Planner::defEasing() const
{
	return ::Anim::Easing(&::Anim::EaseFunc::inOut<&::Anim::EaseFunc::cubic>);
}
