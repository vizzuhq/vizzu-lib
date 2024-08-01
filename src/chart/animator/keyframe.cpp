#include "keyframe.h"

#include <memory>
#include <utility>

#include "base/anim/duration.h"
#include "chart/generator/plot.h"
#include "chart/generator/plotbuilder.h"
#include "chart/generator/plotptr.h"
#include "chart/options/options.h"
#include "dataframe/old/datatable.h"

#include "options.h"

namespace Vizzu::Anim
{

Keyframe::Keyframe(Gen::PlotPtr src,
    const Gen::PlotPtr &trg,
    const Data::DataTable &dataTable,
    const Options::Keyframe *options,
    bool isInstant) :
    options(*options),
    source(std::move(src))
{
	if (isInstant) this->options.all.duration = ::Anim::Duration(0);
	init(trg, dataTable);
	prepareActual();
	createPlan(*source, *target, *actual, this->options);
}

void Keyframe::init(const Gen::PlotPtr &plot,
    const Data::DataTable &dataTable)
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
		source =
		    Gen::PlotBuilder{dataTable, emptyOpt, plot->getStyle()}
		        .build();
		source->keepAspectRatio = plot->keepAspectRatio;
	}
	target = plot;
	target->detachOptions();
}

void Keyframe::prepareActual()
{
	if (Gen::Plot::dimensionMatch(*source, *target)) {
		if (Gen::Plot::hasMarkerChange(*source, *target))
			copyTarget();
		Gen::Plot::mergeMarkersAndCellInfo(*source, *target);
	}
	else {
		copyTarget();
		target->prependMarkers(*source);
		source->appendMarkers(*targetCopy);
	}
	prepareActualMarkersInfo();

	actual = std::make_shared<Gen::Plot>(*source);
	actual->detachOptions();
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

void Keyframe::copyTarget()
{
	if (!targetCopy) {
		targetCopy = target;
		target = std::make_shared<Gen::Plot>(*targetCopy);
		target->detachOptions();
	}
}

}