#include "planner.h"

#include "base/io/log.h"

#include "morph.h"
#include "styles.h"

using namespace Vizzu;
using namespace Vizzu::Anim;
using namespace Vizzu::Diag;
using namespace std::literals::chrono_literals;

void Planner::createPlan(const Diag::Diagram &source,
    const Diag::Diagram &target,
    Diag::Diagram &actual)
{
	this->source = &source;
	this->target = &target;
	this->actual = &actual;

	::Anim::Group::clear();

	Morph::StyleMorphFactory(source.getStyle(), target.getStyle(),
	    actual.getStyle(), *this);

	if (source.getOptions()->title.get() != target.getOptions()->title.get())
		addElement(
		    std::make_unique<::Anim::SingleElement<
		        Diag::DiagramOptions::Title>>(
		        source.getOptions()->title.ref(),
		        target.getOptions()->title.ref(),
		        actual.getOptions()->title.ref()),
		    ::Anim::Options(500ms));

	if (anyMarker(
		[&](const auto &source, const auto &target) -> bool {
		    return (bool)(source.enabled && !target.enabled);
	    }))
		addMorph<Morph::Enable>(1s);

	if (needColor()) addMorph<Morph::Color>(500ms);

	if (source.getOptions()->polar.get()
	        != target.getOptions()->polar.get()
	    || source.getOptions()->angle.get()
	           != target.getOptions()->angle.get())
		addMorph<Morph::CoordinateSystem>(1s);

	const auto &src = source.getOptions()->shapeType.get();
	const auto &trg = target.getOptions()->shapeType.get();
	if((bool)src.getFactor(ShapeType::Circle) && src != trg)
		addMorph<Morph::Shape>(1s);

	if (positionMorphNeeded())
	{
		addMorph<Morph::Vertical>(1s);
		addMorph<Morph::Horizontal>(1s);
	}
	else
	{
		if (verticalBeforeHorizontal())
		{
			if (needVertical()) addMorph<Morph::Vertical>(750ms);
			if (needHorizontal()) addMorph<Morph::Horizontal>(750ms, 750ms);
		}
		else
		{
			if (needHorizontal()) addMorph<Morph::Horizontal>(750ms);
			if (needVertical()) addMorph<Morph::Vertical>(750ms, 750ms);
		}
	}

	if (!(bool)src.getFactor(ShapeType::Circle) && src != trg)
		addMorph<Morph::Shape>(1s);

	if (anyMarker(
		[&](const auto &source, const auto &target) {
			return (bool)(!source.enabled && target.enabled);
		}))
		addMorph<Morph::Enable>(1s);

	if (!source.getOptions()->polar.get()
	    && target.getOptions()->polar.get())
		addMorph<Morph::CoordinateSystem>(1s);
}

template <typename M>
void Planner::addMorph(std::chrono::nanoseconds duration,
    std::chrono::nanoseconds delay)
{
	addElement(
		std::move(std::make_unique<M>(*source, *target, *actual)),
		::Anim::Options(duration, delay)
	);
}

bool Planner::anyMarker(
	const std::function<
		bool(const Diag::Marker &, const Diag::Marker &)
	> &compare) const
{
	for (auto i = 0u;
	     i < source->getItems().size() && i < target->getItems().size();
	     i++)
	{
		if (compare(source->getItems()[i], target->getItems()[i]))
			return true;
	}
	return false;
}

bool Planner::positionMorphNeeded() const
{
	return Diag::canOverlap(
	           (ShapeType::Type)source->getOptions()->shapeType.get())
	    || Diag::canOverlap(
	        (ShapeType::Type)target->getOptions()->shapeType.get());
}

bool Planner::needColor() const
{
	return source->anySelected != target->anySelected
	    || source->getOptions()->legend.get()
	           != target->getOptions()->legend.get()
	    || source->discreteAxises.at(Scale::Type::Color)
	           != target->discreteAxises.at(Scale::Type::Color)
	    || source->discreteAxises.at(Scale::Type::Lightness)
	           != target->discreteAxises.at(Scale::Type::Lightness)
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
		.at(Scales::Id{type, Scales::Index{0}})
		.discretesIds().size();
}

bool Planner::verticalBeforeHorizontal() const
{
	auto srcXcnt = discreteCount(source, Scale::X);
	auto srcYcnt = discreteCount(source, Scale::Y);
	auto trgXcnt = discreteCount(target, Scale::X);
	auto trgYcnt = discreteCount(target, Scale::Y);

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
