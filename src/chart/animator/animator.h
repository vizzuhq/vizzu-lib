#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <functional>
#include <memory>

#include "base/util/eventdispatcher.h"

#include "animation.h"
#include "options.h"

namespace Vizzu::Anim
{

class Animator
{
public:
	Animator(const Data::DataTable &dataTable,
	    const Util::EventDispatcher::Event &onBegin,
	    const Util::EventDispatcher::Event &onComplete);

	void addKeyframe(const Gen::PlotPtr &plot,
	    const Options::Keyframe &options) const;

	void setAnimation(const AnimationPtr &animation);

	void animate(const ::Anim::Control::Option &options,
	    Animation::OnComplete &&onThisCompletes);

	const Data::DataTable &dataTable;
	Util::Event<const Gen::PlotPtr> onDraw;
	Util::Event<> onProgress;
	std::reference_wrapper<const Util::EventDispatcher::Event>
	    onBegin;
	std::reference_wrapper<const Util::EventDispatcher::Event>
	    onComplete;

	::Anim::Control &getControl() { return *actAnimation; }
	AnimationPtr getActAnimation() { return actAnimation; }

private:
	bool running{};
	AnimationPtr actAnimation;
	AnimationPtr nextAnimation;
	void stripActAnimation() const;
	void setupActAnimation() const;
};

}

#endif
