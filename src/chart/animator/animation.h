#ifndef ANIMATION_H
#define ANIMATION_H

#include "base/anim/control.h"
#include "base/anim/sequence.h"
#include "chart/generator/plotptr.h"
#include "chart/options/options.h"

#include "options.h"

namespace Vizzu::Anim
{

class Animation : public ::Anim::Sequence, public ::Anim::Control
{
public:
	using OnComplete = std::function<void(Gen::PlotPtr, bool)>;

	Util::Event<Gen::PlotPtr> onPlotChanged;

	explicit Animation(const Gen::PlotPtr &plot);

	void addKeyframe(const Gen::PlotPtr &next,
	    const Options::Keyframe &options);

	void animate(const ::Anim::Control::Option &options,
	    OnComplete onThisCompletes = OnComplete());

private:
	using Modifier =
	    std::function<void(Gen::Options &, const Gen::Options &)>;

	OnComplete completionCallback;
	Gen::PlotPtr source;
	Gen::PlotPtr target;
	void finish(bool ok);

	static Gen::PlotPtr getIntermediate(const Gen::PlotPtr &base,
	    const Gen::PlotPtr &other,
	    const Modifier &modifier);

	void addKeyframe(const Gen::PlotPtr &source,
	    const Gen::PlotPtr &target,
	    const Options::Keyframe &options,
	    bool isInstant);
};

using AnimationPtr = std::shared_ptr<Animation>;

}

#endif
