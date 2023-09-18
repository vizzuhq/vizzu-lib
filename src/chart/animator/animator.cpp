#include "animator.h"

#include "chart/animator/keyframe.h"
#include "chart/generator/selector.h"

Vizzu::Anim::Animator::Animator() :
    actAnimation(std::make_shared<Animation>(Gen::PlotPtr())),
    nextAnimation(std::make_shared<Animation>(Gen::PlotPtr()))
{}

void Vizzu::Anim::Animator::addKeyframe(const Gen::PlotPtr &plot,
    const Options::Keyframe &options)
{
	nextAnimation->addKeyframe(plot, options);
}

void Vizzu::Anim::Animator::setAnimation(
    const Anim::AnimationPtr &animation)
{
	nextAnimation = animation;
}

void Vizzu::Anim::Animator::animate(const Options::Control &options,
    const Animation::OnComplete &onThisCompletes)
{
	if (running)
		throw std::logic_error("animation already in progress");

	auto completionCallback =
	    [this, onThisCompletes](const Gen::PlotPtr &plot, bool ok)
	{
		nextAnimation = std::make_shared<Animation>(plot);
		this->running = false;
		onThisCompletes(plot, ok);
	};

	running = true;
	stripActAnimation();
	actAnimation = nextAnimation;
	nextAnimation = AnimationPtr();
	setupActAnimation();
	actAnimation->animate(options, completionCallback);
}

void Vizzu::Anim::Animator::setupActAnimation()
{
	actAnimation->onPlotChanged.attach(
	    [this](const Gen::PlotPtr &actual)
	    {
		    onProgress();
		    onDraw(actual);
	    });

	actAnimation->onBegin.attach(onBegin);
	actAnimation->onComplete.attach(onComplete);
}

void Vizzu::Anim::Animator::stripActAnimation()
{
	actAnimation->onPlotChanged.detachAll();
	actAnimation->onBegin.detachAll();
	actAnimation->onComplete.detachAll();
}
