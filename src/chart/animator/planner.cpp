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
	typedef SectionId::EnumType SectionId;

	this->source = &source;
	this->target = &target;
	this->actual = &actual;
	this->options = &options;

	::Anim::Group::clear();

	Morph::StyleMorphFactory(source.getStyle(), target.getStyle(),
	    actual.getStyle(), *this,
		options.get(SectionId::style, defOptions(0.5)));

	if (source.getOptions()->title.get() != target.getOptions()->title.get())
		addElement(
		    std::make_unique<::Anim::SingleElement<
		        Diag::Options::Title>>(
		        source.getOptions()->title.ref(),
		        target.getOptions()->title.ref(),
		        actual.getOptions()->title.ref()),
		    options.get(SectionId::title, 
				defOptions(1, 0, 1, 
				::Anim::Easing(&::Anim::EaseFunc::in<&::Anim::EaseFunc::cubic>))));

	if (source.getOptions()->legend.get() != target.getOptions()->legend.get())
		addElement(
		    std::make_unique<::Anim::SingleElement<
		        Diag::Options::Legend>>(
		        source.getOptions()->legend.ref(),
		        target.getOptions()->legend.ref(),
		        actual.getOptions()->legend.ref()),
		    options.get(SectionId::legend, defOptions(0.5)));

	if (anyMarker(
		[&](const auto &source, const auto &target) -> bool {
		    return (bool)(source.enabled && !target.enabled);
	    }))
		addMorph(SectionId::enable, defOptions(1));

	if (needColor()) addMorph(SectionId::color, defOptions(0.5));

	if (source.getOptions()->polar.get()
	        != target.getOptions()->polar.get()
	    || source.getOptions()->angle.get()
	           != target.getOptions()->angle.get())
		addMorph(SectionId::coordSystem, defOptions(1));

	const auto &src = source.getOptions()->shapeType.get();
	const auto &trg = target.getOptions()->shapeType.get();
	if((bool)src.getFactor(Diag::ShapeType::Circle) && src != trg)
		addMorph(SectionId::geometry, defOptions(1));

	if (positionMorphNeeded())
	{
		addMorph(SectionId::y, defOptions(1));
		addMorph(SectionId::x, defOptions(1));
	}
	else
	{
		auto in = ::Anim::Easing(
			&::Anim::EaseFunc::in<&::Anim::EaseFunc::cubic>);
		auto out = ::Anim::Easing(
			&::Anim::EaseFunc::out<&::Anim::EaseFunc::cubic>);
		if (verticalBeforeHorizontal())
		{
			if (needVertical()) addMorph(SectionId::y, 
				defOptions(1.5, 0, 0.5, in));
			if (needHorizontal()) addMorph(SectionId::x, 
				defOptions(1.5, 0.5, 0.5, out));
		}
		else
		{
			if (needHorizontal()) addMorph(SectionId::x, 
				defOptions(1.5, 0, 0.5, in));
			if (needVertical()) addMorph(SectionId::y, 
				defOptions(1.5, 0.5, 0.5, out));
		}
	}

	if (!(bool)src.getFactor(Diag::ShapeType::Circle) && src != trg)
		addMorph(SectionId::geometry, defOptions(1));

	if (anyMarker(
		[&](const auto &source, const auto &target) {
			return (bool)(!source.enabled && target.enabled);
		}))
		addMorph(SectionId::enable, defOptions(1));

	if (!source.getOptions()->polar.get()
	    && target.getOptions()->polar.get())
		addMorph(SectionId::coordSystem, defOptions(1));
}

void Planner::addMorph(SectionId sectionId, const ::Anim::Options &autoOptions)
{
	addElement(
		Morph::AbstractMorph::create(sectionId, *source, *target, *actual),
		options->get(sectionId, autoOptions)
	);
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
			isAnyLegend(Diag::Scale::Type::Color)
			&&
			(
				source->discreteAxises.at(Diag::Scale::Type::Color)
					!= target->discreteAxises.at(Diag::Scale::Type::Color)
				|| source->axises.at(Diag::Scale::Type::Color)
					!= target->axises.at(Diag::Scale::Type::Color)
			)
		)
		||
		(
			isAnyLegend(Diag::Scale::Type::Lightness)
			&&
			(
				source->discreteAxises.at(Diag::Scale::Type::Lightness)
					!= target->discreteAxises.at(Diag::Scale::Type::Lightness)
				|| source->axises.at(Diag::Scale::Type::Lightness)
					!= target->axises.at(Diag::Scale::Type::Lightness)
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
    Diag::Scale::Type type) const
{
	return diagram->getOptions()->getScales()
		.at(Diag::Scales::Id{type, Diag::Scales::Index{0}})
		.discretesIds().size();
}

bool Planner::verticalBeforeHorizontal() const
{
	auto srcXcnt = discreteCount(source, Diag::Scale::X);
	auto srcYcnt = discreteCount(source, Diag::Scale::Y);
	auto trgXcnt = discreteCount(target, Diag::Scale::X);
	auto trgYcnt = discreteCount(target, Diag::Scale::Y);

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
	return source->axises.at(Diag::Scale::Type::Y)
			!= target->axises.at(Diag::Scale::Type::Y)
		|| source->discreteAxises.at(Diag::Scale::Type::Y)
			!= target->discreteAxises.at(Diag::Scale::Type::Y)
		|| 
		(
			isAnyLegend(Diag::Scale::Type::Size)
			&&
			(
				source->axises.at(Diag::Scale::Type::Size)
					!= target->axises.at(Diag::Scale::Type::Size)
				|| source->discreteAxises.at(Diag::Scale::Type::Size)
					!= target->discreteAxises.at(Diag::Scale::Type::Size)
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
	return source->axises.at(Diag::Scale::Type::X)
	        != target->axises.at(Diag::Scale::Type::X)
	    || source->discreteAxises.at(Diag::Scale::Type::X)
	           != target->discreteAxises.at(Diag::Scale::Type::X)
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

bool Planner::isAnyLegend(Diag::Scale::Type type) const
{
	const auto &src = source->getOptions()->legend.get().get();
	const auto &trg = target->getOptions()->legend.get().get();
	return (src && *src == type)
		|| (trg && *trg == type);
}

::Anim::Options Planner::defOptions(
	double wholeDuration,
	double delayFactor, 
	double durationFactor,
	const ::Anim::Easing &easing) const
{
	::Anim::Options res(::Anim::Duration(1500ms) * wholeDuration, 
		::Anim::Duration(0), easing);
	options->all.writeOver(res);
	if (delayFactor > 0)
		res.delay = res.delay + res.duration * delayFactor;
	if (durationFactor != 1)
		res.duration = res.duration * durationFactor;
	return res;
}
