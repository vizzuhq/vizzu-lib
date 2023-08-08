#ifndef BASE_ANIM_SEQUENCE_H
#define BASE_ANIM_SEQUENCE_H

#include <vector>

#include "base/anim/controllable.h"

namespace Anim
{

class Sequence : public Controllable
{
public:
	Sequence();
	void setPosition(Duration progress) override;
	void addKeyframe(ControllablePtr&& keyframe);

protected:
	std::vector<ControllablePtr> keyframes;
	Controllable *actual {};
};

}

#endif
