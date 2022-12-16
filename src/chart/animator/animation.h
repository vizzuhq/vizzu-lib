#ifndef ANIMATION_H
#define ANIMATION_H

#include "base/anim/sequence.h"
#include "base/anim/control.h"

#include "chart/generator/diagram.h"
#include "options.h"

namespace Vizzu
{
namespace Anim
{

class Animation : 
	public ::Anim::Sequence,
	public ::Anim::Control
{
public:
	typedef std::function<void(Diag::DiagramPtr, bool)> OnComplete;

	Util::Event<Diag::DiagramPtr> onDiagramChanged;

	Animation(const Diag::DiagramPtr &diagram);

	void addKeyframe(
		const Diag::DiagramPtr &next,
		const Options::Keyframe &options);

	void animate(const Options::Control &options,
		OnComplete onThisCompletes = OnComplete());

private:
	OnComplete completionCallback;
	Diag::DiagramPtr source;
	Diag::DiagramPtr target;
	void finish(bool ok);
};

typedef std::shared_ptr<Animation> AnimationPtr;

}
}

#endif
