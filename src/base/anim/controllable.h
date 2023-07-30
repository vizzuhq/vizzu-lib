#ifndef BASE_ANIM_CONTROLABLE_H
#define BASE_ANIM_CONTROLABLE_H

#include <memory>

#include "base/anim/time.h"

namespace Anim
{

class Controllable
{
public:
	virtual ~Controllable() = default;
	virtual void setPosition(Duration progress) = 0;

	[[nodiscard]] const Duration &getDuration() const
	{
		return duration;
	};

	[[nodiscard]] virtual std::shared_ptr<void> data() const
	{
		return nullptr;
	};

protected:
	void setDuration(const Duration& d) {
		duration = d;
	}

	Duration& getSetDuration() {
		return duration;
	}
private:
	Duration duration;

	[[nodiscard]] bool isEmpty() const
	{
		return duration == Duration(0.0);
	}
};

using ControllablePtr = std::shared_ptr<Controllable>;

}

#endif
