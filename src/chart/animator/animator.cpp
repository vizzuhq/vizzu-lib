#include "animator.h"

#include <memory>
#include <stdexcept>
#include <utility>

#include "base/anim/control.h"
#include "base/util/eventdispatcher.h"
#include "chart/generator/plotptr.h"
#include "dataframe/old/types.h"

#include "animation.h"
#include "options.h"

namespace Vizzu::Anim
{

Animator::Animator(const Data::DataTable &dataTable,
    const Util::EventDispatcher::Event &onBegin,
    const Util::EventDispatcher::Event &onComplete) :
    dataTable(dataTable),
    onBegin(onBegin),
    onComplete(onComplete),
    actAnimation(std::make_shared<Animation>()),
    nextAnimation(std::make_shared<Animation>())
{}

void Animator::addKeyframe(const Gen::PlotPtr &plot,
    const Options::Keyframe &options) const
{
	if (running)
		throw std::logic_error("animation already in progress");

	nextAnimation->addKeyframe(plot, dataTable, options);
}

void Animator::setAnimation(const Anim::AnimationPtr &animation)
{
	nextAnimation = animation;
}

void Animator::animate(const ::Anim::Control::Option &options,
    Animation::OnComplete &&onThisCompletes)
{
	if (running)
		throw std::logic_error("animation already in progress");

	onThisCompletes.attach(
	    [this](const Gen::PlotPtr &plot, const bool &)
	    {
		    nextAnimation = std::make_shared<Animation>(plot);
		    this->running = false;
	    });

	running = true;
	stripActAnimation();
	actAnimation = std::exchange(nextAnimation, {});
	setupActAnimation();
	actAnimation->animate(options, std::move(onThisCompletes));
}

void Animator::setupActAnimation() const
{
	actAnimation->onPlotChanged.attach(
	    [this](const Gen::PlotPtr &actual)
	    {
		    onProgress();
		    onDraw(actual);
	    });

	actAnimation->onBegin.attach(
	    [this]
	    {
		    onBegin.get().invoke();
	    });
	actAnimation->onComplete.attach(
	    [this]
	    {
		    onComplete.get().invoke();
	    });
}

void Animator::stripActAnimation() const
{
	actAnimation->onPlotChanged.detachAll();
	actAnimation->onBegin.detachAll();
	actAnimation->onComplete.detachAll();
}

}