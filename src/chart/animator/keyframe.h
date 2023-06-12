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
	Keyframe(Diag::PlotPtr source,
	    Diag::PlotPtr target,
	    const Options::Keyframe &options = Options::Keyframe());

	const Diag::PlotPtr &actualPlot() const { return actual; }

private:
	Options::Keyframe options;
	Diag::PlotPtr source;
	Diag::PlotPtr target;
	Diag::PlotPtr actual;
	Diag::PlotPtr targetCopy;

	void init(Diag::PlotPtr plot);
	void prepareActual();
	void prepareActualMarkersInfo();
	void addMissingMarkers(Diag::PlotPtr source,
	    Diag::PlotPtr target,
	    bool withTargetCopying);
	void copyTarget();
};

}
}

#endif
