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
	typedef std::function<void(Diag::PlotPtr, bool)> OnComplete;

	Util::Event<Diag::PlotPtr> onPlotChanged;

	Animation(const Diag::PlotPtr &plot);

	void addKeyframe(const Diag::PlotPtr &next,
	    const Options::Keyframe &options);

	void animate(const Options::Control &options,
	    OnComplete onThisCompletes = OnComplete());

private:
	typedef std::function<void(Vizzu::Diag::Options &,
	    const Vizzu::Diag::Options &)>
	    Modifier;

	OnComplete completionCallback;
	Diag::PlotPtr source;
	Diag::PlotPtr target;
	void finish(bool ok);

	Diag::PlotPtr getIntermediate(Diag::PlotPtr base,
	    Diag::PlotPtr other,
	    Modifier modifier);

	void addKeyframe(Diag::PlotPtr source,
	    Diag::PlotPtr target,
	    const Options::Keyframe &options,
	    bool canBeInstant = false);
};

typedef std::shared_ptr<Animation> AnimationPtr;

}
}

#endif
