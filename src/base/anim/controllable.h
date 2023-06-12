#ifndef ANIM_CONTROLABLE
#define ANIM_CONTROLABLE

#include <memory>

#include "base/anim/time.h"

namespace Anim
{

class Controllable
{
public:
	virtual ~Controllable() {}
	virtual void setPosition(Duration progress) = 0;

	Duration getDuration() const { return duration; };

protected:
	Duration duration;

	bool isEmpty() const { return duration == Duration(0.0); }
};

typedef std::shared_ptr<Controllable> ControllablePtr;

}

#endif
