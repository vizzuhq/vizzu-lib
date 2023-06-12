#include "keyframe.h"

using namespace Vizzu;
using namespace Vizzu::Anim;

Keyframe::Keyframe(Diag::PlotPtr src,
    Diag::PlotPtr trg,
    const Options::Keyframe &options) :
    options(options),
    source(src)
{
	init(trg);
	prepareActual();
	createPlan(*source, *target, *actual, this->options);
}

void Keyframe::init(Diag::PlotPtr plot)
{
	if (plot) {
		if ((!source || source->isEmpty()) && plot) {
			auto emptyOpt = std::make_shared<Diag::Options>(
			    *plot->getOptions());
			emptyOpt->reset();
			if (source && source->getOptions()->title.get().get())
				emptyOpt->title.set(
				    source->getOptions()->title.get());
			source =
			    std::make_shared<Diag::Plot>(plot->getTable(),
			        emptyOpt,
			    plot->getStyle(),
			        false);
			source->keepAspectRatio = plot->keepAspectRatio;
		}
		target = plot;
		target->detachOptions();
	}
}

void Keyframe::prepareActual()
{
	if (Diag::Plot::dimensionMatch(*source, *target)) {
		addMissingMarkers(source, target, true);

		prepareActualMarkersInfo();
	}
	else {
		copyTarget();
		/*
		        auto sourceSize = source->getMarkers().size();
		        for (auto &markerInfo: target->markersInfo)
		            markerInfo.second.values[0].value.markerId +=
		   sourceSize;
		*/
		target->prependMarkers(*source, false);
		source->appendMarkers(*targetCopy, false);

		prepareActualMarkersInfo();
	}

	auto options =
	    std::make_shared<Diag::Options>(*source->getOptions());

	actual = std::make_shared<Diag::Plot>(options, *source);

	actual->markers = source->getMarkers();
	actual->markersInfo = source->getMarkersInfo();
}

void Keyframe::prepareActualMarkersInfo()
{
	auto &origTMI = target->getMarkersInfo();
	auto &smi = source->getMarkersInfo();
	for (auto &item : smi) {
		auto iter = origTMI.find(item.first);
		if (iter != origTMI.end()) {
			copyTarget();
			target->getMarkersInfo().insert(
			    std::make_pair(item.first, item.second));
		}
		else {
			copyTarget();
			target->getMarkersInfo().insert(std::make_pair(item.first,
			    Diag::Plot::MarkerInfo{}));
		}
	}
	for (auto &item : origTMI) {
		auto iter = smi.find(item.first);
		if (iter != smi.end())
			smi.insert(std::make_pair(item.first, item.second));
		else
			smi.insert(std::make_pair(item.first,
			    Diag::Plot::MarkerInfo{}));
	}
}

void Keyframe::addMissingMarkers(Diag::PlotPtr source,
    Diag::PlotPtr target,
    bool withTargetCopying)
{
	for (auto i = source->getMarkers().size();
	     i < target->getMarkers().size();
	     i++) {
		auto src = target->getMarkers()[i];
		src.enabled = false;
		source->markers.push_back(src);
	}

	for (auto i = target->getMarkers().size();
	     i < source->getMarkers().size();
	     i++) {
		if (withTargetCopying) copyTarget();
		auto trg = source->getMarkers()[i];
		trg.enabled = false;
		target->markers.push_back(trg);
	}
}

void Keyframe::copyTarget()
{
	if (!targetCopy) {
		targetCopy = target;
		target = std::make_shared<Diag::Plot>(*targetCopy);
		target->detachOptions();
	}
}
