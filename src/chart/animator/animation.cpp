#include "animation.h"

#include <utility>

#include "chart/animator/keyframe.h"

using namespace Vizzu;
using namespace Vizzu::Anim;
using namespace std::chrono;

Animation::Animation(const Gen::PlotPtr &plot) :
    ::Anim::Control(static_cast<Controllable &>(*this)),
    source(plot),
    target(plot)
{
	::Anim::Control::setOnChange(
	    [&]
	    {
		    if (!::Anim::Sequence::actual) return;
		    auto plot = ::Anim::Sequence::actual->data();
		    if (!plot) return;
		    onPlotChanged(
		        std::static_pointer_cast<Gen::Plot>(std::move(plot)));
	    });

	::Anim::Control::setOnFinish(
	    [&](bool ok)
	    {
		    finish(ok);
	    });
}

void Animation::addKeyframe(const Gen::PlotPtr &next,
    const Options::Keyframe &options)
{
	if (isRunning())
		throw std::logic_error("animation already in progress");

	if (!next) return;
	next->detachOptions();

	auto strategy = options.getRegroupStrategy();

	if (!target || target->isEmpty() || !next || next->isEmpty()
	    || Gen::Plot::dimensionMatch(*target, *next)
	    || target->getOptions()->sameShadow(*next->getOptions())) {
		strategy = RegroupStrategy::fade;
	}

	Vizzu::Gen::PlotPtr intermediate0;
	Vizzu::Gen::PlotPtr intermediate1;

	if (strategy == RegroupStrategy::drilldown) {
		intermediate0 = getIntermediate(target,
		    next,
		    [=](auto &base, const auto &other)
		    {
			    base.drilldownTo(other);
		    });

		intermediate1 = getIntermediate(next,
		    target,
		    [=](auto &base, const auto &other)
		    {
			    base.drilldownTo(other);
		    });
	}
	else if (strategy == RegroupStrategy::aggregate) {
		auto &&andFilter = Data::Filter(
		    [srcFilter = target->getOptions()->dataFilter,
		        trgFilter = next->getOptions()->dataFilter](
		        const Data::RowWrapper &row)
		    {
			    return srcFilter.match(row) && trgFilter.match(row);
		    },
		    0);

		auto loosingCoordsys =
		    target->getOptions()->getChannels().anyAxisSet()
		    && !next->getOptions()->getChannels().anyAxisSet();

		auto gainingCoordsys =
		    !target->getOptions()->getChannels().anyAxisSet()
		    && next->getOptions()->getChannels().anyAxisSet();

		auto geometryChanges = target->getOptions()->geometry
		                    != next->getOptions()->geometry;

		auto basedOnSource =
		    loosingCoordsys || (!gainingCoordsys && geometryChanges);

		auto getModifier = [andFilter = std::move(andFilter)](
		                       bool drilldownToBase)
		{
			return [andFilter, drilldownToBase](auto &base,
			           const auto &target)
			{
				auto baseCopy = base;
				base.intersection(target);
				base.drilldownTo(drilldownToBase ? baseCopy : target);
				base.dataFilter = andFilter;
			};
		};

		if (basedOnSource) {
			intermediate0 =
			    getIntermediate(target, next, getModifier(true));
			intermediate1 =
			    getIntermediate(target, next, getModifier(false));
		}
		else {
			intermediate0 =
			    getIntermediate(next, target, getModifier(false));
			intermediate1 =
			    getIntermediate(next, target, getModifier(true));
		}
	}

	auto begin = target;
	if (intermediate0) {
		addKeyframe(target,
		    intermediate0,
		    options,
		    strategy == RegroupStrategy::drilldown);
		begin = intermediate0;
	}
	if (intermediate1) {
		addKeyframe(begin,
		    intermediate1,
		    options,
		    strategy == RegroupStrategy::aggregate);
		begin = intermediate1;
	}
	addKeyframe(begin,
	    next,
	    options,
	    strategy == RegroupStrategy::drilldown);

	target = next;
}

Gen::PlotPtr Animation::getIntermediate(const Gen::PlotPtr &base,
    const Gen::PlotPtr &other,
    const std::function<void(Vizzu::Gen::Options &,
        const Vizzu::Gen::Options &)> &modifier)
{
	Gen::PlotPtr res;

	auto extOptions =
	    std::make_shared<Gen::Options>(*base->getOptions());

	modifier(*extOptions, *other->getOptions());

	if (*extOptions != *other->getOptions()
	    && *extOptions != *base->getOptions()) {
		res = std::make_shared<Gen::Plot>(base->getTable(),
		    extOptions,
		    base->getStyle(),
		    false);

		res->keepAspectRatio = base->keepAspectRatio;
	}
	return res;
}

void Animation::addKeyframe(const Gen::PlotPtr& source,
    const Gen::PlotPtr& target,
    const Options::Keyframe &options,
    bool canBeInstant)
{
	auto instant = options;
	instant.all.duration = ::Anim::Duration(0);

	auto keyframe = std::make_shared<Keyframe>(source,
	    target,
	    canBeInstant
	            && source->getOptions()->looksTheSame(
	                *target->getOptions())
	        ? instant
	        : options);
	::Anim::Sequence::addKeyframe(keyframe);
}

void Animation::animate(const Options::Control &options,
    OnComplete onThisCompletes)
{
	if (isRunning())
		throw std::logic_error("animation already in progress");

	completionCallback = std::move(onThisCompletes);
	::Anim::Control::reset();
	::Anim::Control::setPlayState(options.playState);
	::Anim::Control::setDirection(options.direction);
	::Anim::Control::seekProgress(options.position);
	onBegin();
}

void Animation::finish(bool ok)
{
	onComplete();
	auto f = completionCallback;
	completionCallback = OnComplete();
	if (f)
		f(ok ? (::Anim::Control::atEndPosition() ? target : source)
		     : source,
		    ok);
}
