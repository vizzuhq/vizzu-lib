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

	Animation(const Data::DataTable &dataTable,
	    const Gen::PlotPtr &plot);

	void addKeyframe(const Gen::PlotPtr &next,
	    const Options::Keyframe &options);

	void animate(const ::Anim::Control::Option &options,
	    OnComplete &&onThisCompletes);

private:
	const Data::DataTable &dataTable;
	OnComplete completionCallback;
	Gen::PlotPtr source;
	Gen::PlotPtr target;

	template <class Modifier>
	Gen::PlotPtr getIntermediate(const Gen::PlotPtr &base,
	    const Gen::PlotPtr &other,
	    Modifier &&modifier) const;

	void addKeyframe(const Gen::PlotPtr &source,
	    const Gen::PlotPtr &target,
	    const Options::Keyframe &options,
	    bool isInstant);
};

using AnimationPtr = std::shared_ptr<Animation>;

}

#endif
