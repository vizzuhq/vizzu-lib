#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <functional>
#include <memory>

#include "base/anim/sequence.h"
#include "chart/generator/plot.h"

#include "animation.h"
#include "options.h"

namespace Vizzu::Anim
{

class Animator
{
public:
	Animator();
	Animator(const Animator &) = delete;

	void addKeyframe(const Gen::PlotPtr &plot,
	    const Options::Keyframe &options = Options::Keyframe());

	void setAnimation(const Anim::AnimationPtr &animation);

	void animate(const Options::Control &options = Options::Control(),
	    const Animation::OnComplete &onThisCompletes =
	        Animation::OnComplete());

	Util::Event<Gen::PlotPtr> onDraw;
	Util::Event<> onProgress;
	std::function<void()> onBegin;
	std::function<void()> onComplete;

	::Anim::Control &getControl() { return *actAnimation; }
	AnimationPtr getActAnimation() { return actAnimation; }

private:
	bool running{};
	AnimationPtr actAnimation;
	AnimationPtr nextAnimation;
	void stripActAnimation();
	void setupActAnimation();
};

}

#endif
