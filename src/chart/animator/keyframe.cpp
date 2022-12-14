#include "keyframe.h"

using namespace Vizzu;
using namespace Vizzu::Anim;

Keyframe::Keyframe(
	Diag::DiagramPtr source, 
	Diag::DiagramPtr target,
	const Options::Keyframe &options)
	: options(options),
	  source(source),
	  target(std::make_shared<Diag::Diagram>(*target))
{
	target->detachOptions();
	
	init();
}

void Keyframe::init()
{
	prepareSource();
	auto hasVirtuals = prepareVirtualCharts();
	auto needSimpleFade = !hasVirtuals 
		&& !Diag::Diagram::dimensionMatch(*source, *target);
	prepareActual();
	createPlan(*source, *target, *actual, options, needSimpleFade);
}

void Keyframe::prepareSource()
{
	if (!source || source->isEmpty())
	{
		auto emptyOpt = std::make_shared<Diag::Options>(*target->getOptions());
		emptyOpt->reset();
		if (source) emptyOpt->title.set(source->getOptions()->title.get());
		source = std::make_shared<Diag::Diagram>(target->getTable(), 
			emptyOpt, target->getStyle(), false);
		source->keepAspectRatio = target->keepAspectRatio;
	}
}

bool Keyframe::prepareVirtualCharts()
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
				virtualTarget ? virtualTarget : target);

			return true;
		}
	}
	return false;
}

void Keyframe::prepareActual()
{
	if(Diag::Diagram::dimensionMatch(*source, *target))
	{
		addMissingMarkers(source, target);

		prepareActualMarkersInfo();
	}
	else
	{
		if (!virtualSource && !virtualTarget)
		{
			target->prependMarkers(*source, false);
			source->appendMarkers(*target, false);
		}
		else if (virtualSource && virtualTarget)
		{
/*			auto sourceSize = source->getMarkers().size();
			for (auto &markerInfo: target->markersInfo)
				markerInfo.second.values[0].value.markerId += sourceSize;
*/
			target->prependMarkers(*source, false);
			source->appendMarkers(*target, false);

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

void Keyframe::prepareActualMarkersInfo() 
{
	auto& origTMI = target->getMarkersInfo();
	auto& smi = source->getMarkersInfo();
	for(auto& item : smi) {
		auto iter = origTMI.find(item.first);
		if (iter != origTMI.end()) {
			target->getMarkersInfo().insert(std::make_pair(item.first, item.second));
		}
		else {
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

void Keyframe::addMissingMarkers(
		Diag::DiagramPtr source,
		Diag::DiagramPtr target)
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
		auto trg = source->getMarkers()[i];
		trg.enabled = false;
		target->markers.push_back(trg);
	}
}



