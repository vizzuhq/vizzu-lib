#include "animation.h"

#include <utility>

#include "chart/animator/keyframe.h"
#include "chart/generator/plot.h"
#include "chart/generator/plotbuilder.h"

namespace Vizzu::Anim
{

Animation::Animation(const Gen::PlotPtr &plot) :
    ::Anim::Control(static_cast<Controllable &>(*this)),
    source(plot),
    target(plot)
{
	onChange.attach(
	    [this]
	    {
		    if (!::Anim::Sequence::actual) return;
		    auto plot = ::Anim::Sequence::actual->data();
		    if (!plot) return;
		    onPlotChanged(
		        std::static_pointer_cast<Gen::Plot>(std::move(plot)));
	    });

	onFinish.attach(
	    [this](const bool &ok)
	    {
		    onComplete();

		    completionCallback(ok && atEndPosition() ? target
		                                             : source,
		        ok);
		    completionCallback.detachAll();
	    });
}

void Animation::addKeyframe(const Gen::PlotPtr &next,
    const Data::DataTable &dataTable,
    const Options::Keyframe &options)
{
	if (isRunning())
		throw std::logic_error("animation already in progress");

	if (!next) return;
	next->detachOptions();

	auto strategy = options.getRegroupStrategy();

	if (!target || target->isEmpty() || next->isEmpty()
	    || Gen::Plot::dimensionMatch(*target, *next)
	    || target->getOptions()->sameShadow(*next->getOptions())) {
		strategy = RegroupStrategy::fade;
	}

	Vizzu::Gen::PlotPtr intermediate0;
	Vizzu::Gen::PlotPtr intermediate1;

	if (strategy == RegroupStrategy::drilldown) {
		auto drilldown = +[](Vizzu::Gen::Options &base,
		                      const Vizzu::Gen::Options &other)
		{
			base.drilldownTo(other);
		};
		intermediate0 =
		    getIntermediate(target, next, dataTable, drilldown);

		intermediate1 =
		    getIntermediate(next, target, dataTable, drilldown);
	}
	else if (strategy == RegroupStrategy::aggregate) {
		auto &&targetAxisSet =
		    target->getOptions()->getChannels().anyAxisSet();
		auto &&nextAxisSet =
		    next->getOptions()->getChannels().anyAxisSet();

		auto &&loosingCoordsys = targetAxisSet && !nextAxisSet;
		auto &&gainingCoordsys = !targetAxisSet && nextAxisSet;

		auto &&geometryChanges = target->getOptions()->geometry
		                      != next->getOptions()->geometry;

		auto &&basedOnSource =
		    loosingCoordsys || (!gainingCoordsys && geometryChanges);

		auto &&getModifier =
		    [andFilter = target->getOptions()->dataFilter
		              && next->getOptions()->dataFilter](
		        bool drilldownToBase)
		{
			return [&andFilter,
			           drilldownToBase](Vizzu::Gen::Options &base,
			           const Vizzu::Gen::Options &target)
			{
				auto baseCopy = base;
				base.intersection(target);
				base.drilldownTo(drilldownToBase ? baseCopy : target);
				base.dataFilter = andFilter;
			};
		};

		const auto &base = basedOnSource ? target : next;
		const auto &other = basedOnSource ? next : target;
		intermediate0 = getIntermediate(base,
		    other,
		    dataTable,
		    getModifier(basedOnSource));
		intermediate1 = getIntermediate(base,
		    other,
		    dataTable,
		    getModifier(!basedOnSource));
	}

	auto &&intermediate0Instant = intermediate0
	                           && strategy != RegroupStrategy::fade
	                           && target->getOptions()->looksTheSame(
	                               *intermediate0->getOptions());
	auto begin = std::ref(intermediate0 ? intermediate0 : target);

	auto &&intermediate1Instant =
	    intermediate1 && strategy == RegroupStrategy::aggregate
	    && begin.get()->getOptions()->looksTheSame(
	        *intermediate1->getOptions());
	begin = intermediate1 ? std::ref(intermediate1) : begin;

	auto &&nextInstant = strategy != RegroupStrategy::fade
	                  && begin.get()->getOptions()->looksTheSame(
	                      *next->getOptions());

	auto &&duration_fix = (intermediate0 && !intermediate0Instant)
	                    + (intermediate1 && !intermediate1Instant)
	                    + !nextInstant;

	auto real_options = options;
	if (auto &duration = real_options.all.duration;
	    duration_fix > 1 && duration) {
		*duration =
		    ::Anim::Duration{static_cast<double>(duration.value())
		                     / static_cast<double>(duration_fix)};
	}

	begin = target;

	if (intermediate0) {
		addKeyframe(begin,
		    intermediate0,
		    dataTable,
		    real_options,
		    intermediate0Instant);
		begin = intermediate0;

		if (!intermediate0Instant)
			real_options.all.delay = ::Anim::Duration(0);
	}
	if (intermediate1) {
		addKeyframe(begin,
		    intermediate1,
		    dataTable,
		    real_options,
		    intermediate1Instant);
		begin = intermediate1;

		if (!intermediate1Instant)
			real_options.all.delay = ::Anim::Duration(0);
	}
	addKeyframe(begin, next, dataTable, real_options, nextInstant);

	target = next;
}

template <class Modifier>
Gen::PlotPtr Animation::getIntermediate(const Gen::PlotPtr &base,
    const Gen::PlotPtr &other,
    const Data::DataTable &dataTable,
    Modifier &&modifier)
{
	Gen::PlotPtr res;

	auto extOptions =
	    std::make_shared<Gen::Options>(*base->getOptions());

	modifier(*extOptions, *other->getOptions());

	if (*extOptions != *other->getOptions()
	    && *extOptions != *base->getOptions()) {
		res =
		    Gen::PlotBuilder{dataTable, extOptions, base->getStyle()}
		        .build();

		res->keepAspectRatio = base->keepAspectRatio;
	}
	return res;
}

void Animation::addKeyframe(const Gen::PlotPtr &source,
    const Gen::PlotPtr &target,
    const Data::DataTable &dataTable,
    const Options::Keyframe &options,
    bool isInstant)
{
	Sequence::addKeyframe(std::make_shared<Keyframe>(source,
	    target,
	    dataTable,
	    &options,
	    isInstant));
}

void Animation::animate(const ::Anim::Control::Option &options,
    OnComplete &&onThisCompletes)
{
	if (isRunning())
		throw std::logic_error("animation already in progress");

	completionCallback = std::move(onThisCompletes);
	::Anim::Control::reset();
	this->options = options;
	onBegin();
}

}