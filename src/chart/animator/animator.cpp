#include "animator.h"

#include "chart/animator/keyframe.h"

namespace Vizzu::Anim
{

Animator::Animator() :
    actAnimation(std::make_shared<Animation>(Gen::PlotPtr())),
    nextAnimation(std::make_shared<Animation>(Gen::PlotPtr()))
{}

void Animator::addKeyframe(const Gen::PlotPtr &plot,
    const Options::Keyframe &options) const
{
	if (running)
		throw std::logic_error("animation already in progress");

	nextAnimation->addKeyframe(plot, options);
}

void Animator::setAnimation(const Anim::AnimationPtr &animation)
{
	nextAnimation = animation;
}

void Animator::animate(const ::Anim::Control::Option &options,
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
	actAnimation = std::exchange(nextAnimation, {});
	setupActAnimation();
	actAnimation->animate(options, completionCallback);
}

void Animator::setupActAnimation() const
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

void Animator::stripActAnimation() const
{
	actAnimation->onPlotChanged.detachAll();
	actAnimation->onBegin.detachAll();
	actAnimation->onComplete.detachAll();
}

}