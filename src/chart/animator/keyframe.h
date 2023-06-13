#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <memory>

#include "chart/generator/plot.h"

#include "options.h"
#include "planner.h"

namespace Vizzu
{
namespace Anim
{

class Keyframe : public Planner
{
public:
	Keyframe(Gen::PlotPtr source,
	    Gen::PlotPtr target,
	    const Options::Keyframe &options = Options::Keyframe());

	const Gen::PlotPtr &actualPlot() const { return actual; }

private:
	Options::Keyframe options;
	Gen::PlotPtr source;
	Gen::PlotPtr target;
	Gen::PlotPtr actual;
	Gen::PlotPtr targetCopy;

	void init(Gen::PlotPtr plot);
	void prepareActual();
	void prepareActualMarkersInfo();
	void addMissingMarkers(Gen::PlotPtr source,
	    Gen::PlotPtr target,
	    bool withTargetCopying);
	void copyTarget();
};

}
}

#endif
