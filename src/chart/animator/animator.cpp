#include "animator.h"

#include "base/io/log.h"
#include "chart/generator/selector.h"

using namespace Vizzu;
using namespace Vizzu::Anim;
using namespace std::chrono;

Animator::Animator() : ::Anim::Control(static_cast<Planner&>(*this))
{
	source = nullptr;
}

void Animator::init(Diag::DiagramPtr diagram)
{
	if (!source) {
		source = std::move(diagram);
		source->detachOptions();

		::Anim::Control::setOnChange([&]
		{
			if (onDraw) onDraw(actual);
		});

		::Anim::Control::setOnFinish([&] { finish(); });
	}
}

void Animator::animate(const Diag::DiagramPtr &diagram,
    OnFinished onThisFinished)
{
	if (isRunning()) throw std::logic_error("animation already in progress");
	if (!diagram) return;

	diagram->detachOptions();

	init(diagram);
	target = diagram;
	if (onThisFinished) onFinished.attach(onThisFinished, true);
	Diag::Selector(*target).copySelectionFrom(*source);
	target->detachOptions();
	prepareActual();
	createPlan(*source, *target, *actual);
	::Anim::Control::reset();
	::Anim::Control::play();
}

void Animator::finish()
{
	source = targetCopy ? targetCopy : target;
	actual.reset();
	target.reset();
	targetCopy.reset();
	onFinished();
}

void Animator::prepareActual()
{
	auto options =
	    std::make_shared<Diag::DiagramOptions>(*source->getOptions());

	actual = std::make_shared<Diag::Diagram>(options, *source);

	if(Diag::Diagram::dimensionMatch(*source, *target))
	{
		for (auto i = source->getItems().size();
			i < target->getItems().size();
			i++)
		{
			auto srcItem = target->getItems()[i];
			srcItem.enabled = false;
			source->items.push_back(srcItem);
		}

		for (auto i = target->getItems().size();
		     i < source->getItems().size();
		     i++)
		{
			if (!targetCopy) copyTarget();
			auto trgItem = source->getItems()[i];
			trgItem.enabled = false;
			target->items.push_back(trgItem);
		}
	}
	else
	{
		copyTarget();

		auto sourceSize = source->getItems().size();

		for (auto &item: target->items)
			item.setIdOffset(sourceSize);

		source->items.insert(source->items.end(),
			target->getItems().begin(), target->getItems().end());

		target->items = source->getItems();

		for (auto i = 0u; i < source->getItems().size(); i++)
		{
			auto &item = (i < sourceSize ? target : source)->items[i];
			item.enabled = false;
		}
	}
	actual->items = source->getItems();
}

void Animator::copyTarget()
{
	targetCopy = target;
	target = std::make_shared<Diag::Diagram>(*targetCopy);
	target->detachOptions();
}
