#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <memory>
#include <functional>

#include "chart/generator/diagram.h"

#include "planner.h"
#include "options.h"

namespace Vizzu
{
namespace Anim
{

class Animator :
    public ::Anim::Control,
    public Planner
{
public:
	typedef std::function<void(Diag::DiagramPtr)> OnDraw;
	typedef std::function<void()> OnProgress;
	typedef std::function<void(Diag::DiagramPtr)> OnComplete;

	Animator();
	Animator(const Animator &) = delete;

	void animate(const Diag::DiagramPtr &diagram,
	    Options &&options = Options(),
	    OnComplete onThisCompletes = OnComplete());

	OnDraw onDraw;
	OnProgress onProgress;

private:
	Diag::DiagramPtr source;
	Diag::DiagramPtr target;
	Diag::DiagramPtr targetCopy;
	Diag::DiagramPtr actual;
	OnComplete onComplete;
	void init(Diag::DiagramPtr diagram);
	void finish();
	void prepareActual();
	void prepareActualMarkersInfo();
	void copyTarget();
};

}
}

#endif
