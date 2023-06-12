#ifndef ANIM_SEQUENCE
#define ANIM_SEQUENCE

#include <vector>

#include "base/anim/controllable.h"

namespace Anim
{

class Sequence : public Controllable
{
public:
	Sequence();
	void setPosition(Duration progress) override;
	void addKeyframe(ControllablePtr keyframe);

protected:
	std::vector<ControllablePtr> keyframes;
	Controllable *actual;
};

}

#endif
