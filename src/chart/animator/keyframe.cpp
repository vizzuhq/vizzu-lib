#include "keyframe.h"

#include <utility>

#include "chart/generator/plot.h"

namespace Vizzu::Anim
{

Keyframe::Keyframe(Gen::PlotPtr src,
    const Gen::PlotPtr &trg,
    const Options::Keyframe *options,
    bool isInstant) :
    options(*options),
    source(std::move(src))
{
	if (isInstant) this->options.all.duration = ::Anim::Duration(0);
	init(trg);
	prepareActual();
	createPlan(*source, *target, *actual, this->options);
}

void Keyframe::init(const Gen::PlotPtr &plot)
{
	if (!plot) return;

	if (!source || source->isEmpty()) {
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
		addMissingMarkers(source, target);

		mergeMarkerCellInfo(source, target);

		prepareActualMarkersInfo();
	}
	else {
		copyTarget();

		target->prependMarkers(*source);
		source->appendMarkers(*targetCopy);

		prepareActualMarkersInfo();
	}

	auto options =
	    std::make_shared<Gen::Options>(*source->getOptions());

	actual = std::make_shared<Gen::Plot>(options, *source);
}

void Keyframe::prepareActualMarkersInfo()
{
	const auto &origTMI = target->getMarkersInfo();
	auto &smi = source->getMarkersInfo();
	if (!smi.empty()) copyTarget();

	for (auto &tmi = target->getMarkersInfo(); auto &&item : smi)
		tmi.insert(std::pair{item.first, Gen::Plot::MarkerInfo{}});

	for (auto &&item : origTMI)
		smi.insert(std::pair{item.first, Gen::Plot::MarkerInfo{}});
}

void Keyframe::addMissingMarkers(const Gen::PlotPtr &source,
    const Gen::PlotPtr &target)
{
	auto &&smarkers = source->markers;
	auto &&tmarkers = target->markers;
	auto &&ssize = smarkers.size();
	auto &&tsize = tmarkers.size();
	for (auto i = ssize; i < tsize; ++i)
		smarkers.emplace_back(tmarkers[i]).enabled = false;

	if (tsize < ssize) copyTarget();
	for (auto i = tsize; i < ssize; ++i)
		target->markers.emplace_back(smarkers[i]).enabled = false;
}

void Keyframe::mergeMarkerCellInfo(const Gen::PlotPtr &source,
    const Gen::PlotPtr &target)
{
	const auto markers_size = source->markers.size();
	for (std::size_t ix{}; ix < markers_size; ++ix)
		if (auto &scell = source->markers[ix].cellInfo,
		    &tcell = target->markers[ix].cellInfo;
		    scell && !tcell)
			tcell = scell;
		else if (!scell && tcell)
			scell = tcell;
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