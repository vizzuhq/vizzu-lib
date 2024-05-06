#include "keyframe.h"

#include <utility>

#include "chart/generator/plot.h"

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
		    plot->getStyle());
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
	const auto &origTMI = target->getMarkersInfo();
	auto &smi = source->getMarkersInfo();
	if (!smi.empty()) copyTarget();

	for (auto &tmi = target->getMarkersInfo(); auto &&item : smi)
		if (origTMI.contains(item.first))
			tmi.insert({item.first, item.second});
		else
			tmi.insert(
			    std::pair{item.first, Gen::Plot::MarkerInfo{}});

	for (auto &&item : origTMI)
		smi.insert(std::pair{item.first, Gen::Plot::MarkerInfo{}});
}

void Keyframe::addMissingMarkers(const Gen::PlotPtr &source,
    const Gen::PlotPtr &target,
    bool withTargetCopying)
{
	auto &&smarkers = source->markers;
	auto &&tmarkers = target->markers;
	auto &&ssize = smarkers.size();
	auto &&tsize = tmarkers.size();
	for (auto i = ssize; i < tsize; ++i)
		smarkers.emplace_back(tmarkers[i]).enabled = false;

	if (tsize < ssize && withTargetCopying) copyTarget();
	for (auto i = tsize; i < ssize; ++i)
		target->markers.emplace_back(smarkers[i]).enabled = false;
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