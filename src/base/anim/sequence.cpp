#include "sequence.h"

using namespace Anim;

Sequence::Sequence() = default;

void Sequence::setPosition(Duration progress)
{
	auto start = Duration(0);

	if (progress > getDuration()) progress = getDuration();

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

void Sequence::addKeyframe(ControllablePtr&& keyframe)
{
	getSetDuration() += keyframe->getDuration();
	keyframes.emplace_back(std::move(keyframe));
}
