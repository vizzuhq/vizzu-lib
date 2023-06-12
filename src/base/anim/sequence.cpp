#include "sequence.h"

using namespace Anim;

Sequence::Sequence() : actual(nullptr) {}

void Sequence::setPosition(Duration progress)
{
	auto start = Duration(0);

	if (progress > duration) progress = duration;

	for (const auto &keyframe : keyframes) {
		actual = keyframe.get();
		if (progress - start > actual->getDuration()) {
			start += actual->getDuration();
		}
		else {
			actual->setPosition(progress - start);
			return;
		}
	}
}

void Sequence::addKeyframe(ControllablePtr keyframe)
{
	keyframes.push_back(keyframe);
	duration += keyframe->getDuration();
}
