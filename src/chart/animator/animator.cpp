#include "animator.h"

#include "base/io/log.h"
#include "chart/animator/keyframe.h"
#include "chart/generator/selector.h"

using namespace Vizzu;
using namespace Vizzu::Anim;
using namespace std::chrono;

Animator::Animator() : 
	running(false)
{
	actAnimation = std::make_shared<Animation>(Diag::DiagramPtr());
	nextAnimation = std::make_shared<Animation>(Diag::DiagramPtr());
}

void Animator::addKeyframe(const Diag::DiagramPtr &diagram,
    const Options::Keyframe &options)
{
	nextAnimation->addKeyframe(diagram, options);
}

void Animator::animate(
	const Options::Control &options,
	Animation::OnComplete onThisCompletes)
{
	if (running) throw std::logic_error("animation already in progress");

	auto completionCallback = [=](Diag::DiagramPtr diagram, bool ok) {
		nextAnimation = std::make_shared<Animation>(diagram);
		this->running = false;
		onThisCompletes(diagram, ok);
	};

	running = true;
	stripActAnimation();
	actAnimation = nextAnimation;
	nextAnimation = AnimationPtr();
	setupActAnimation();
	actAnimation->animate(options, completionCallback);
}

void Animator::setupActAnimation()
{
	actAnimation->onDiagramChanged.attach([&](const Diag::DiagramPtr &actual)
	{
		onProgress();
		onDraw(actual);
	});

	actAnimation->onBegin.attach(onBegin);
	actAnimation->onComplete.attach(onComplete);
}

void Animator::stripActAnimation()
{
	actAnimation->onDiagramChanged.detachAll();
	actAnimation->onBegin.detachAll();
	actAnimation->onComplete.detachAll();
}
