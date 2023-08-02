#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <memory>

#include "chart/generator/plot.h"

#include "options.h"
#include "planner.h"

namespace Vizzu::Anim
{

class Keyframe : public Planner
{
public:
	Keyframe(Gen::PlotPtr src,
	    const Gen::PlotPtr &trg,
	    Options::Keyframe options = Options::Keyframe());

	[[nodiscard]] std::shared_ptr<void> data() const final
	{
		return actual;
	}

private:
	Options::Keyframe options;
	Gen::PlotPtr source;
	Gen::PlotPtr target;
	Gen::PlotPtr actual;
	Gen::PlotPtr targetCopy;

	void init(const Gen::PlotPtr &plot);
	void prepareActual();
	void prepareActualMarkersInfo();
	void addMissingMarkers(const Gen::PlotPtr& source,
	    const Gen::PlotPtr& target,
	    bool withTargetCopying);
	void copyTarget();
};

}

#endif
