#include "keyframe.h"

#include <utility>

namespace Vizzu::Anim
{

Keyframe::Keyframe(Gen::PlotPtr src,
    const Gen::PlotPtr &trg,
    Options::Keyframe options) :
    options(std::move(options)),
    source(std::move(src))
{
	init(trg);
	prepareActual();
	createPlan(*source, *target, *actual, this->options);
}

void Keyframe::init(const Gen::PlotPtr &plot)
{
	if (!plot) return;

	if ((!source || source->isEmpty())) {
		auto emptyOpt =
		    std::make_shared<Gen::Options>(*plot->getOptions());
		emptyOpt->reset();
		if (source) {
			if (auto &&title = source->getOptions()->title.get())
				emptyOpt->title = title;
			if (auto &&subtitle =
			        source->getOptions()->subtitle.get())
				emptyOpt->subtitle = subtitle;
			if (auto &&caption = source->getOptions()->caption.get())
				emptyOpt->caption = caption;
		}
		source = std::make_shared<Gen::Plot>(plot->getTable(),
		    emptyOpt,
		    plot->getStyle(),
		    false);
		source->keepAspectRatio = plot->keepAspectRatio;
	}
	target = plot;
	target->detachOptions();
}

void Keyframe::prepareActual()
{
	if (Gen::Plot::dimensionMatch(*source, *target)) {
		addMissingMarkers(source, target, true);

		prepareActualMarkersInfo();
	}
	else {
		copyTarget();

		target->prependMarkers(*source, false);
		source->appendMarkers(*targetCopy, false);

		prepareActualMarkersInfo();
	}

	auto options =
	    std::make_shared<Gen::Options>(*source->getOptions());

	actual = std::make_shared<Gen::Plot>(options, *source);

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
			target->getMarkersInfo().insert(
			    std::make_pair(item.first, Gen::Plot::MarkerInfo{}));
		}
	}
	for (auto &item : origTMI) {
		auto iter = smi.find(item.first);
		if (iter != smi.end())
			smi.insert(std::make_pair(item.first, item.second));
		else
			smi.insert(
			    std::make_pair(item.first, Gen::Plot::MarkerInfo{}));
	}
}

void Keyframe::addMissingMarkers(const Gen::PlotPtr &source,
    const Gen::PlotPtr &target,
    bool withTargetCopying)
{
	for (auto i = source->getMarkers().size();
	     i < target->getMarkers().size();
	     ++i) {
		auto src = target->getMarkers()[i];
		src.enabled = false;
		source->markers.push_back(src);
	}

	for (auto i = target->getMarkers().size();
	     i < source->getMarkers().size();
	     ++i) {
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
		target = std::make_shared<Gen::Plot>(*targetCopy);
		target->detachOptions();
	}
}

}