#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <memory>

#include "chart/generator/diagram.h"

#include "options.h"
#include "planner.h"

namespace Vizzu
{
namespace Anim
{

class Keyframe : public Planner
{
public:
	Keyframe(Diag::DiagramPtr source,
	    Diag::DiagramPtr target,
	    const Options::Keyframe &options = Options::Keyframe());

	const Diag::DiagramPtr &actualDiagram() const { return actual; }

private:
	Options::Keyframe options;
	Diag::DiagramPtr source;
	Diag::DiagramPtr target;
	Diag::DiagramPtr actual;
	Diag::DiagramPtr targetCopy;

	void init(Diag::DiagramPtr diagram);
	void prepareActual();
	void prepareActualMarkersInfo();
	void addMissingMarkers(Diag::DiagramPtr source,
	    Diag::DiagramPtr target,
	    bool withTargetCopying);
	void copyTarget();
};

}
}

#endif
