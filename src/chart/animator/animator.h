#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <memory>
#include <functional>

#include "base/util/event.h"
#include "chart/generator/diagram.h"

#include "planner.h"

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
	typedef std::function<void()> OnFinished;

	Animator();
	Animator(const Animator &) = delete;

	void init(Diag::DiagramPtr diagram);
	void animate(const Diag::DiagramPtr &diagram,
	    OnFinished onFinished = OnFinished());

	Util::Event<> onFinished;
	OnDraw onDraw;

private:
	Diag::DiagramPtr source;
	Diag::DiagramPtr target;
	Diag::DiagramPtr targetCopy;
	Diag::DiagramPtr actual;

	void finish();
	void prepareActual();
	void copyTarget();
};

}
}

#endif
