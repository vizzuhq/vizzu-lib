#include "animation.h"

#include "chart/animator/keyframe.h"

using namespace Vizzu;
using namespace Vizzu::Anim;
using namespace std::chrono;

Animation::Animation(const Diag::DiagramPtr &diagram) :
	::Anim::Control(dynamic_cast<Sequence&>(*this)),
	source(diagram),
	target(diagram)
{
	::Anim::Control::setOnChange([&]
	{
		if (!::Anim::Sequence::actual) return;
		auto keyframe = dynamic_cast<Keyframe*>(::Anim::Sequence::actual);
		if (!keyframe) return;
		onDiagramChanged(keyframe->actualDiagram());
	});

	::Anim::Control::setOnFinish([&](bool ok) { finish(ok); });
}

void Animation::addKeyframe(
	const Diag::DiagramPtr &next,
    const Options::Keyframe &options)
{
	if (isRunning()) throw std::logic_error("animation already in progress");

	if (!next) return;
	next->detachOptions();

	auto strategy = options.getRegroupStrategy();

	if (!target || target->isEmpty()
		|| !next || next->isEmpty()
		|| Diag::Diagram::dimensionMatch(*target, *next)
		|| target->getOptions()->sameShadow(*next->getOptions())) 
	{
		strategy = RegroupStrategy::fade;
	}

	Vizzu::Diag::DiagramPtr intermediate0;
	Vizzu::Diag::DiagramPtr intermediate1;

	if (strategy == RegroupStrategy::drilldown)
	{
		intermediate0 = getIntermediate(target, next, 
			[=](auto &base, const auto &other) { 
				base.drilldownTo(other);
			});

		intermediate1 = getIntermediate(next, target, 
			[=](auto &base, const auto &other) { 
				base.drilldownTo(other); 
			});
	}
	else if (strategy == RegroupStrategy::aggregate)
	{
		Vizzu::Data::Filter srcFilter = target->getOptions()->dataFilter.get();
		Vizzu::Data::Filter trgFilter = next->getOptions()->dataFilter.get();

		auto andFilter = Data::Filter([=](const Data::RowWrapper &row){
			return srcFilter.match(row) && trgFilter.match(row);
		}, 0);

		auto loosingCoordsys = target->getOptions()->getScales().anyAxisSet()
			&& !next->getOptions()->getScales().anyAxisSet();

		auto gainingCoordsys = !target->getOptions()->getScales().anyAxisSet()
			&& next->getOptions()->getScales().anyAxisSet();

		auto geometryChanges = target->getOptions()->shapeType.get()
			!= next->getOptions()->shapeType.get();

		auto basedOnSource = loosingCoordsys 
			|| (!gainingCoordsys && geometryChanges);

		auto getModifier = [=](bool drilldownToBase)
		{
			return [=](auto &base, const auto &target) { 
				auto baseCopy = base; 
				base.intersection(target);
				base.drilldownTo(drilldownToBase ? baseCopy : target);
				base.dataFilter.set(andFilter);
			};
		};

		if (basedOnSource)
		{
			intermediate0 = getIntermediate(target, next, getModifier(true));
			intermediate1 = getIntermediate(target, next, getModifier(false));
		} 
		else 
		{
			intermediate0 = getIntermediate(next, target, getModifier(false));
			intermediate1 = getIntermediate(next, target, getModifier(true));
		}
	}

	auto begin = target;
	if (intermediate0) {
		addKeyframe(target, intermediate0, options,
			strategy == RegroupStrategy::drilldown);
		begin = intermediate0;
	}
	if (intermediate1) {
		addKeyframe(begin, intermediate1, options,
			strategy == RegroupStrategy::aggregate);
		begin = intermediate1;
	}
	addKeyframe(begin, next, options,
		strategy == RegroupStrategy::drilldown);

	target = next;
}

Diag::DiagramPtr Animation::getIntermediate(
	Diag::DiagramPtr base, 
	Diag::DiagramPtr other,
	std::function<
		void(Vizzu::Diag::Options &,const Vizzu::Diag::Options &)
	> modifier)
{
	Diag::DiagramPtr res;

	auto extOptions =
		std::make_shared<Diag::Options>(*base->getOptions());

	modifier(*extOptions, *other->getOptions());

	if (*extOptions != *other->getOptions()
		&& *extOptions != *base->getOptions())
	{
		res = std::make_shared<Diag::Diagram>(
			base->getTable(), 
			extOptions, 
			base->getStyle(), 
			false);

		res->keepAspectRatio = base->keepAspectRatio;
	}
	return res;
}

void Animation::addKeyframe(
	Diag::DiagramPtr source, 
	Diag::DiagramPtr target,
	const Options::Keyframe &options,
	bool canBeInstant)
{
	auto instant = options;
	instant.all.duration = ::Anim::Duration(0);

	auto keyframe = std::make_shared<Keyframe>(source, target, 
		canBeInstant && source->getOptions()->looksTheSame(*target->getOptions())
		? instant : options);
	::Anim::Sequence::addKeyframe(keyframe);
}

void Animation::animate(const Options::Control &options,
		OnComplete onThisCompletes)
{
	if (isRunning()) throw std::logic_error("animation already in progress");

	completionCallback = onThisCompletes;
	::Anim::Control::reset();
	::Anim::Control::setPlayState(options.playState);
	::Anim::Control::setDirection(options.direction);
	::Anim::Control::seekProgress(options.position);
	onBegin();
}

void Animation::finish(bool ok)
{
	onComplete();
	auto f = completionCallback;
	completionCallback = OnComplete();
	if (f) f(
		ok ? (::Anim::Control::atEndPosition() ? target : source) : source, 
		ok);
}

