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
	using OnComplete = Util::Event<const Gen::PlotPtr, const bool>;

	Util::Event<const Gen::PlotPtr> onPlotChanged;

	explicit Animation(const Gen::PlotPtr &plot = {});

	void addKeyframe(const Gen::PlotPtr &next,
	    const Data::DataTable &dataTable,
	    const Options::Keyframe &options);

	void animate(const Options &options,
	    OnComplete &&onThisCompletes);

private:
	OnComplete completionCallback;
	Gen::PlotPtr source;
	Gen::PlotPtr target;

	void reTime(::Anim::Duration duration, ::Anim::Duration delay);

	template <class Modifier>
	static Gen::PlotPtr getIntermediate(const Gen::PlotPtr &base,
	    const Gen::PlotPtr &other,
	    const Data::DataTable &dataTable,
	    Modifier &&modifier);

	void addKeyframe(const Gen::PlotPtr &source,
	    const Gen::PlotPtr &target,
	    const Data::DataTable &dataTable,
	    const Options::Keyframe &options,
	    bool isInstant);
};

using AnimationPtr = std::shared_ptr<Animation>;

}

#endif
