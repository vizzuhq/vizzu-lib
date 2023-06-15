#ifndef ANIMATION_H
#define ANIMATION_H

#include "base/anim/control.h"
#include "base/anim/sequence.h"
#include "chart/generator/plot.h"

#include "options.h"

namespace Vizzu
{
namespace Anim
{

class Animation : public ::Anim::Sequence, public ::Anim::Control
{
public:
	typedef std::function<void(Gen::PlotPtr, bool)> OnComplete;

	Util::Event<Gen::PlotPtr> onPlotChanged;

	Animation(const Gen::PlotPtr &plot);

	void addKeyframe(const Gen::PlotPtr &next,
	    const Options::Keyframe &options);

	void animate(const Options::Control &options,
	    OnComplete onThisCompletes = OnComplete());

private:
	typedef std::function<void(Vizzu::Gen::Options &,
	    const Vizzu::Gen::Options &)>
	    Modifier;

	OnComplete completionCallback;
	Gen::PlotPtr source;
	Gen::PlotPtr target;
	void finish(bool ok);

	Gen::PlotPtr getIntermediate(Gen::PlotPtr base,
	    Gen::PlotPtr other,
	    Modifier modifier);

	void addKeyframe(Gen::PlotPtr source,
	    Gen::PlotPtr target,
	    const Options::Keyframe &options,
	    bool canBeInstant = false);
};

typedef std::shared_ptr<Animation> AnimationPtr;

}
}

#endif
