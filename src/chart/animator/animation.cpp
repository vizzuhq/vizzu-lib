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
	auto keyframe = std::make_shared<Keyframe>(target, next, options);
	::Anim::Sequence::addKeyframe(keyframe);
	target = next;
}

void Animation::animate(const Options::Control &options,
		OnComplete onThisCompletes)
{
	if (isRunning()) throw std::logic_error("animation already in progress");

	completionCallback = onThisCompletes;
	::Anim::Control::reset();
	::Anim::Control::setPlayState(options.playState);
	::Anim::Control::seekProgress(options.position);
	onBegin();
}

void Animation::finish(bool ok)
{
	onComplete();
	auto f = completionCallback;
	completionCallback = OnComplete();
	if (f) f(ok ? target : source, ok);
}

