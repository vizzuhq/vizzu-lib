#include "animator.h"

#include "base/io/log.h"
#include "chart/generator/selector.h"

using namespace Vizzu;
using namespace Vizzu::Anim;
using namespace std::chrono;

Animator::Animator() : ::Anim::Control(static_cast<Planner&>(*this))
{
	::Anim::Control::setOnChange([&]
	{
		onProgress();
		onDraw(actual);
	});

	::Anim::Control::setOnFinish([&](bool ok) { finish(ok); });
}

void Animator::init(Diag::DiagramPtr diagram)
{
	if (target)
		source = targetCopy ? targetCopy : target;

	actual.reset();
	target.reset();
	targetCopy.reset();
	virtualSource.reset();
	virtualTarget.reset();

	if (diagram)
	{
		if ((!source || source->isEmpty()) && diagram)
		{
			auto emptyOpt = std::make_shared<Diag::Options>(*diagram->getOptions());
			emptyOpt->reset();
			if (source) emptyOpt->title.set(source->getOptions()->title.get());
			source = std::make_shared<Diag::Diagram>(diagram->getTable(), 
				emptyOpt, diagram->getStyle(), false);
			source->keepAspectRatio = diagram->keepAspectRatio;
		}
		target = diagram;
		target->detachOptions();
	}
}

void Animator::cancel()
{
	target.reset();
	auto diagram = source;
	source.reset();
	init(diagram);
}

void Animator::animate(const Diag::DiagramPtr &diagram,
    Options &&options,
    OnComplete onThisCompletes)
{
	if (isRunning()) throw std::logic_error("animation already in progress");
	if (!diagram) return;

	diagram->detachOptions();
	init(diagram);
	completionCallback = onThisCompletes;
	auto hasVirtuals = prepareVirtualCharts();
	auto needSimpleFade = !hasVirtuals 
		&& !Diag::Diagram::dimensionMatch(*source, *target);
	prepareActual();
	createPlan(*source, *target, *actual, options, needSimpleFade);
	::Anim::Control::reset();
	::Anim::Control::setPlayState(options.playState);
	::Anim::Control::seekProgress(options.position);
	onBegin();
}

void Animator::finish(bool ok)
{
	onComplete();
	if (!ok) cancel();
	auto f = completionCallback;
	completionCallback = OnComplete();
	if (f) f(targetCopy ? targetCopy : target, ok);
}

bool Animator::prepareVirtualCharts()
{
	if(!source->isEmpty()
		&& !target->isEmpty()
		&& !Diag::Diagram::dimensionMatch(*source, *target)
		&& !source->getOptions()->sameShadow(*target->getOptions()))
	{
		auto extSrcOptions =
			std::make_shared<Diag::Options>(*source->getOptions());

		auto extTrgOptions =
			std::make_shared<Diag::Options>(*target->getOptions());

		auto srcDimensions = source->getOptions()->getScales().getDimensions();
		auto trgDimensions = target->getOptions()->getScales().getDimensions();

		auto &srcStackAxis = extSrcOptions->stackAxis();
		auto &trgStackAxis = extTrgOptions->stackAxis();

		for (const auto &dim: trgDimensions)
		{
			if (!extSrcOptions->getScales().isSeriesUsed(dim))
				srcStackAxis.addSeries(dim);
		}

		for (const auto &dim: srcDimensions)
		{
			if (!extTrgOptions->getScales().isSeriesUsed(dim))
				trgStackAxis.addSeries(dim);
		}

		if (*extSrcOptions != *source->getOptions()
			&& *extSrcOptions != *target->getOptions())
		{
			virtualSource = std::make_shared<Diag::Diagram>(
				source->getTable(), 
				extSrcOptions, 
				source->getStyle(), 
				false);

			virtualSource->keepAspectRatio = source->keepAspectRatio;

			for (auto &marker: virtualSource->markers) marker.isVirtual = true;
		}

		if (*extTrgOptions != *source->getOptions()
			&& *extTrgOptions != *target->getOptions())
		{
			virtualTarget = std::make_shared<Diag::Diagram>(
				target->getTable(), 
				extTrgOptions, 
				target->getStyle(), 
				false);

			virtualTarget->keepAspectRatio = target->keepAspectRatio;

			for (auto &marker: virtualTarget->markers) marker.isVirtual = true;
		}

		if (virtualSource || virtualTarget)
		{
			addMissingMarkers(
				virtualSource ? virtualSource : source, 
				virtualTarget ? virtualTarget : target, 
				!virtualTarget);

			return true;
		}
	}
	return false;
}

void Animator::prepareActual()
{
	if(Diag::Diagram::dimensionMatch(*source, *target))
	{
		addMissingMarkers(source, target, true);

		prepareActualMarkersInfo();
	}
	else
	{
		copyTarget();

		if (!virtualSource && !virtualTarget)
		{
			target->prependMarkers(*source, false);
			source->appendMarkers(*targetCopy, false);
		}
		else if (virtualSource && virtualTarget)
		{
/*			auto sourceSize = source->getMarkers().size();
			for (auto &markerInfo: target->markersInfo)
				markerInfo.second.values[0].value.markerId += sourceSize;
*/
			target->prependMarkers(*source, false);
			source->appendMarkers(*targetCopy, false);

			source->appendMarkers(*virtualSource, true);
			target->appendMarkers(*virtualTarget, true);
		}
		else if (virtualSource && !virtualTarget)
		{
			target->prependMarkers(*source, false);
			source->appendMarkers(*virtualSource, true);
		}
		else if (!virtualSource && virtualTarget)
		{
			source->appendMarkers(*target, false);
			target->prependMarkers(*virtualTarget, true);
		}

		prepareActualMarkersInfo();
	}

	auto options =
	    std::make_shared<Diag::Options>(*source->getOptions());

	actual = std::make_shared<Diag::Diagram>(options, *source);
	
	actual->markers = source->getMarkers();
	actual->markersInfo = source->getMarkersInfo();
}

void Animator::addMissingMarkers(
		Diag::DiagramPtr source,
		Diag::DiagramPtr target,
		bool withTargetCopying)
{
	for (auto i = source->getMarkers().size();
		i < target->getMarkers().size();
		i++)
	{
		auto src = target->getMarkers()[i];
		src.enabled = false;
		source->markers.push_back(src);
	}

	for (auto i = target->getMarkers().size();
			i < source->getMarkers().size();
			i++)
	{
		if (withTargetCopying) copyTarget();
		auto trg = source->getMarkers()[i];
		trg.enabled = false;
		target->markers.push_back(trg);
	}
}

void Animator::prepareActualMarkersInfo() {
	auto& origTMI = target->getMarkersInfo();
	auto& smi = source->getMarkersInfo();
	for(auto& item : smi) {
		auto iter = origTMI.find(item.first);
		if (iter != origTMI.end()) {
			copyTarget();
			target->getMarkersInfo().insert(std::make_pair(item.first, item.second));
		}
		else {
			copyTarget();
			target->getMarkersInfo().insert(std::make_pair(item.first, Diag::Diagram::MarkerInfo{}));
		}
	}
	for(auto& item : origTMI) {
		auto iter = smi.find(item.first);
		if (iter != smi.end())
			smi.insert(std::make_pair(item.first, item.second));
		else
			smi.insert(std::make_pair(item.first, Diag::Diagram::MarkerInfo{}));
	}
}

void Animator::copyTarget()
{
	if (!targetCopy)
	{
		targetCopy = target;
		target = std::make_shared<Diag::Diagram>(*targetCopy);
		target->detachOptions();
	}
}
