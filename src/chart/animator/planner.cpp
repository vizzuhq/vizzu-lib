#include "planner.h"

#include "base/anim/easingfunc.h"
#include "chart/generator/plot.h"

#include "morph.h"
#include "styles.h"

namespace Vizzu::Anim
{

void Planner::createPlan(const Gen::Plot &source,
    const Gen::Plot &target,
    Gen::Plot &actual,
    const Options::Keyframe &options)
{
	using std::literals::chrono_literals::operator""ms;
	using std::literals::chrono_literals::operator""s;

	this->source = &source;
	this->target = &target;
	this->actual = &actual;
	this->options = &options;

	const auto &srcOpt = source.getOptions();
	const auto &trgOpt = target.getOptions();
	const auto &actOpt = actual.getOptions();

	reset();
	calcNeeded();

	const ::Anim::Duration baseStep(1125ms);
	::Anim::Duration step(baseStep);

	if (Gen::Plot::dimensionMatch(source, target)) {
		addMorph(SectionId::hide, baseStep);

		setBaseline();

		::Anim::Duration delay;
		::Anim::Duration xdelay;
		::Anim::Duration posDuration;

		::Anim::Easing in3(
		    &::Anim::EaseFunc::in<&::Anim::EaseFunc::cubic>);
		::Anim::Easing out3(
		    &::Anim::EaseFunc::out<&::Anim::EaseFunc::cubic>);
		::Anim::Easing inOut3(
		    &::Anim::EaseFunc::inOut<&::Anim::EaseFunc::cubic>);
		const ::Anim::Easing inOut5(
		    &::Anim::EaseFunc::inOut<&::Anim::EaseFunc::quint>);

		if (positionMorphNeeded()) {
			if (animNeeded[SectionId::x] && animNeeded[SectionId::y])
				step = 1600ms;

			addMorph(SectionId::x, step);
			addMorph(SectionId::y, step);
			posDuration += step;
		}
		else {
			auto first = verticalBeforeHorizontal() ? SectionId::y
			                                        : SectionId::x;
			auto second =
			    first == SectionId::y ? SectionId::x : SectionId::y;

			if (animNeeded[first]) {
				addMorph(first,
				    step,
				    0ms,
				    animNeeded[second] ? in3 : inOut3);
				delay = step;
				posDuration += step;
			}

			if (animNeeded[second]) {
				addMorph(second,
				    step,
				    delay,
				    animNeeded[first] ? out3 : inOut3);
				if (second == SectionId::x) xdelay = delay;
				posDuration += step;
			}
		}

		addMorph(SectionId::connection,
		    (animNeeded[SectionId::coordSystem]
		        || animNeeded[SectionId::geometry])
		            && getDuration() == getBaseline()
		        ? step
		        : getDuration() - getBaseline());

		if (animNeeded[SectionId::style])
			Morph::StyleMorphFactory(source.getStyle(),
			    target.getStyle(),
			    actual.getStyle())
			    .populate(*this, getOptions(SectionId::style, step));

		if (animNeeded[SectionId::legend])
			addElement(
			    std::make_unique<
			        ::Anim::SingleElement<Gen::Options::Legend>>(
			        srcOpt->legend,
			        trgOpt->legend,
			        actOpt->legend),
			    getOptions(SectionId::legend, step));

		addMorph(SectionId::color, step);
		addMorph(SectionId::coordSystem, std::max(step, posDuration));

		const auto &geomEasing =
		    srcOpt->geometry == Gen::ShapeType::circle   ? in3
		    : trgOpt->geometry == Gen::ShapeType::circle ? out3
		    : srcOpt->geometry == Gen::ShapeType::line   ? in3
		    : trgOpt->geometry == Gen::ShapeType::line   ? out3
		                                                 : inOut5;

		addMorph(SectionId::geometry,
		    std::max(step, posDuration),
		    0ms,
		    geomEasing);

		setBaseline();

		addMorph(SectionId::show, baseStep);
	}
	else {
		if (animNeeded[SectionId::show]
		    && animNeeded[SectionId::hide]) {
			::Anim::Easing in3(
			    &::Anim::EaseFunc::in<&::Anim::EaseFunc::cubic>);

			addMorph(SectionId::show, step);
			addMorph(SectionId::hide, step, 0ms, in3);
		}
		else if (animNeeded[SectionId::show])
			addMorph(SectionId::show, step);
		else if (animNeeded[SectionId::hide])
			addMorph(SectionId::hide, step);

		resetBaseline();

		addMorph(SectionId::x, step);
		addMorph(SectionId::y, step);
		addMorph(SectionId::connection, step);

		if (animNeeded[SectionId::style])
			Morph::StyleMorphFactory(source.getStyle(),
			    target.getStyle(),
			    actual.getStyle())
			    .populate(*this, getOptions(SectionId::style, step));

		if (animNeeded[SectionId::legend])
			addElement(
			    std::make_unique<
			        ::Anim::SingleElement<Gen::Options::Legend>>(
			        srcOpt->legend,
			        trgOpt->legend,
			        actOpt->legend),
			    getOptions(SectionId::legend, step));

		addMorph(SectionId::color, step);
		addMorph(SectionId::coordSystem, step);
		addMorph(SectionId::geometry, step);
	}

	resetBaseline();

	for (auto &&[section, getter] :
	    {std::pair{SectionId::title, &Gen::Options::title},
	        {SectionId::subtitle, &Gen::Options::subtitle},
	        {SectionId::caption, &Gen::Options::caption}}) {
		if (animNeeded[section]) {
			auto duration = static_cast<double>(getDuration()) > 0
			                  ? getDuration()
			                  : 1s;

			auto &src = std::invoke(getter, srcOpt);
			auto &trg = std::invoke(getter, trgOpt);

			::Anim::Easing easing = defEasing();
			if (src.get() && trg.get())
				easing = ::Anim::Easing{&::Anim::EaseFunc::middle<
				    &::Anim::EaseFunc::quint>};

			auto &&options =
			    getOptions(section, duration, 0s, easing);

			addElement(
			    std::make_unique<
			        ::Anim::SingleElement<Gen::Options::Heading>>(src,
			        trg,
			        std::invoke(getter, actOpt)),
			    options);
		}
	}

	if (animNeeded[SectionId::tooltip]) {
		addElement(std::make_unique<
		               ::Anim::SingleElement<Gen::Plot::MarkersInfo>>(
		               source.getMarkersInfo(),
		               target.getMarkersInfo(),
		               actual.getMarkersInfo()),
		    getOptions(SectionId::tooltip, 300ms));
	}

	if (options.all.duration.has_value()
	    && options.all.duration->msec() > 0
	    && std::ranges::none_of(animNeeded, std::identity{})) {
		setDuration(*options.all.duration
		            + options.all.delay.value_or(::Anim::Duration()));
	}

	reTime();
}

void Planner::reTime()
{
	using std::literals::chrono_literals::operator""s;

	if (options->all.duration && options->all.delay)
		::Anim::Group::reTime(*options->all.duration,
		    *options->all.delay);
	else if (options->all.duration)
		::Anim::Group::reTime(*options->all.duration, 0s);
	else if (options->all.delay
	         && static_cast<double>(getDuration()) != 0.0)
		::Anim::Group::reTime(getDuration(), *options->all.delay);
}

void Planner::reset()
{
	::Anim::Group::clear();

	for (auto i = 0U; i < std::size(animNeeded); ++i)
		animNeeded[static_cast<SectionId>(i)] = false;
}

void Planner::calcNeeded()
{
	const auto &srcOpt = source->getOptions();
	const auto &trgOpt = target->getOptions();

	animNeeded[SectionId::style] =
	    Morph::StyleMorphFactory(source->getStyle(),
	        target->getStyle(),
	        actual->getStyle())
	        .isNeeded();

	animNeeded[SectionId::title] = srcOpt->title != trgOpt->title;
	animNeeded[SectionId::subtitle] =
	    srcOpt->subtitle != trgOpt->subtitle;
	animNeeded[SectionId::caption] =
	    srcOpt->caption != trgOpt->caption;
	animNeeded[SectionId::tooltip] =
	    srcOpt->markersInfo != trgOpt->markersInfo;

	animNeeded[SectionId::legend] =
	    (static_cast<bool>(srcOpt->legend.get())
	        != static_cast<bool>(trgOpt->legend.get()))
	    || (static_cast<bool>(srcOpt->legend.get())
	        && static_cast<bool>(trgOpt->legend.get())
	        && (*srcOpt->legend.get() != *trgOpt->legend.get()));

	animNeeded[SectionId::show] =
	    anyMarker(+[](const Gen::Marker &source,
	                   const Gen::Marker &target) -> bool
	        {
		        return static_cast<bool>(
		            !source.enabled && target.enabled);
	        });

	animNeeded[SectionId::hide] =
	    anyMarker(+[](const Gen::Marker &source,
	                   const Gen::Marker &target) -> bool
	        {
		        return static_cast<bool>(
		            source.enabled && !target.enabled);
	        });

	animNeeded[SectionId::color] = needColor();

	animNeeded[SectionId::coordSystem] =
	    srcOpt->coordSystem != trgOpt->coordSystem
	    || srcOpt->angle != trgOpt->angle;

	animNeeded[SectionId::geometry] =
	    srcOpt->geometry != trgOpt->geometry;

	animNeeded[SectionId::y] = needVertical();
	animNeeded[SectionId::x] = needHorizontal();

	animNeeded[SectionId::connection] =
	    anyMarker(+[](const Gen::Marker &source,
	                   const Gen::Marker &target) -> bool
	        {
		        return source.prevMainMarker != target.prevMainMarker;
	        })
	    || srcOpt->isHorizontal() != trgOpt->isHorizontal();
}

bool Planner::anyMarker(
    bool (*compare)(const Gen::Marker &, const Gen::Marker &)) const
{
	for (auto i = 0U; i < source->getMarkers().size()
	                  && i < target->getMarkers().size();
	     ++i) {
		if (compare(source->getMarkers()[i], target->getMarkers()[i]))
			return true;
	}
	return false;
}

bool Planner::positionMorphNeeded() const
{
	typedef Gen::ShapeType ST;

	auto &srcShape = source->getOptions()->geometry;
	auto &trgShape = target->getOptions()->geometry;

	auto anyCircle = srcShape == ST::circle || trgShape == ST::circle;

	if (anyCircle) return true;

	auto anyRectangle =
	    srcShape == ST::rectangle || trgShape == ST::rectangle;

	return !anyRectangle;
}

bool Planner::needColor() const
{
	return (isAnyLegend(Gen::ChannelId::color)
	           && (source->axises.at(Gen::ChannelId::color).common
	                   != target->axises.at(Gen::ChannelId::color)
	                          .common
	               || source->axises.at(Gen::ChannelId::color)
	                          .dimension
	                      != target->axises.at(Gen::ChannelId::color)
	                             .dimension
	               || source->axises.at(Gen::ChannelId::color).measure
	                      != target->axises.at(Gen::ChannelId::color)
	                             .measure))
	    || (isAnyLegend(Gen::ChannelId::lightness)
	        && (source->axises.at(Gen::ChannelId::lightness).common
	                != target->axises.at(Gen::ChannelId::lightness)
	                       .common
	            || source->axises.at(Gen::ChannelId::lightness)
	                       .dimension
	                   != target->axises.at(Gen::ChannelId::lightness)
	                          .dimension
	            || source->axises.at(Gen::ChannelId::lightness)
	                       .measure
	                   != target->axises.at(Gen::ChannelId::lightness)
	                          .measure))
	    || anyMarker(+[](const Gen::Marker &source,
	                      const Gen::Marker &target) -> bool
	        {
		        return (source.enabled || target.enabled)
		            && source.colorBase != target.colorBase;
	        });
}

size_t Planner::dimensionCount(const Gen::Plot *plot,
    Gen::ChannelId type)
{
	return plot->getOptions()
	    ->getChannels()
	    .at(type)
	    .dimensions()
	    .size();
}

bool Planner::verticalBeforeHorizontal() const
{
	const auto &srcOpt = source->getOptions();
	const auto &trgOpt = target->getOptions();

	if (srcOpt->isHorizontal() != trgOpt->isHorizontal()
	    || !srcOpt->getChannels().anyAxisSet()
	    || !trgOpt->getChannels().anyAxisSet()) {
		if (srcOpt->getChannels().anyAxisSet())
			return srcOpt->subAxisType() == Gen::ChannelId::y;
		if (trgOpt->getChannels().anyAxisSet())
			return trgOpt->mainAxisType() == Gen::ChannelId::y;
	}

	auto srcXcnt = dimensionCount(source, Gen::ChannelId::x);
	auto srcYcnt = dimensionCount(source, Gen::ChannelId::y);
	auto trgXcnt = dimensionCount(target, Gen::ChannelId::x);
	auto trgYcnt = dimensionCount(target, Gen::ChannelId::y);

	if ((trgYcnt != srcYcnt) || (trgXcnt != srcXcnt)) {
		return (trgYcnt > srcYcnt) || (trgXcnt < srcXcnt);
	}
	return !source->getOptions()->isHorizontal();
}

bool Planner::needVertical() const
{
	return source->axises.at(Gen::ChannelId::y).common
	        != target->axises.at(Gen::ChannelId::y).common
	    || source->axises.at(Gen::ChannelId::y).measure
	           != target->axises.at(Gen::ChannelId::y).measure
	    || source->axises.at(Gen::ChannelId::y).dimension
	           != target->axises.at(Gen::ChannelId::y).dimension
	    || source->guides.at(Gen::ChannelId::y)
	           != target->guides.at(Gen::ChannelId::y)
	    || (isAnyLegend(Gen::ChannelId::size)
	        && (source->axises.at(Gen::ChannelId::size).common
	                != target->axises.at(Gen::ChannelId::size).common
	            || source->axises.at(Gen::ChannelId::size).measure
	                   != target->axises.at(Gen::ChannelId::size)
	                          .measure
	            || source->axises.at(Gen::ChannelId::size).dimension
	                   != target->axises.at(Gen::ChannelId::size)
	                          .dimension))
	    || source->anyAxisSet != target->anyAxisSet
	    || (source->markerConnectionOrientation
	            != target->markerConnectionOrientation
	        && (source->markerConnectionOrientation.value_or(
	                Gen::Orientation::horizontal)
	                == Gen::Orientation::vertical
	            || target->markerConnectionOrientation.value_or(
	                   Gen::Orientation::horizontal)
	                   == Gen::Orientation::vertical))
	    || source->axises.at(Gen::ChannelId::label)
	               .measure.origMeasureName.get()
	           != target->axises.at(Gen::ChannelId::label)
	                  .measure.origMeasureName.get()
	    || source->axises.at(Gen::ChannelId::label).measure.unit.get()
	           != target->axises.at(Gen::ChannelId::label)
	                  .measure.unit.get()
	    || anyMarker(+[](const Gen::Marker &source,
	                      const Gen::Marker &target) -> bool
	        {
		        return (source.enabled || target.enabled)
		            && (source.position.y != target.position.y
		                || source.spacing.y != target.spacing.y
		                || source.size.y != target.size.y
		                || source.sizeFactor != target.sizeFactor
		                || source.label != target.label);
	        });
}

bool Planner::needHorizontal() const
{
	return source->axises.at(Gen::ChannelId::x).common
	        != target->axises.at(Gen::ChannelId::x).common
	    || source->axises.at(Gen::ChannelId::x).measure
	           != target->axises.at(Gen::ChannelId::x).measure
	    || source->axises.at(Gen::ChannelId::x).dimension
	           != target->axises.at(Gen::ChannelId::x).dimension
	    || source->guides.at(Gen::ChannelId::x)
	           != target->guides.at(Gen::ChannelId::x)
	    || source->anyAxisSet != target->anyAxisSet
	    || source->keepAspectRatio != target->keepAspectRatio
	    || (source->markerConnectionOrientation
	            != target->markerConnectionOrientation
	        && ((source->markerConnectionOrientation.value_or(
	                 Gen::Orientation::vertical)
	                == Gen::Orientation::horizontal)
	            || (target->markerConnectionOrientation.value_or(
	                    Gen::Orientation::vertical)
	                == Gen::Orientation::horizontal)))
	    || anyMarker(+[](const Gen::Marker &source,
	                      const Gen::Marker &target) -> bool
	        {
		        return (source.enabled || target.enabled)
		            && (source.position.x != target.position.x
		                || source.spacing.x != target.spacing.x
		                || source.size.x != target.size.x);
	        });
}

bool Planner::isAnyLegend(Gen::ChannelId type) const
{
	const auto &src = source->getOptions()->legend.get();
	const auto &trg = target->getOptions()->legend.get();
	return (src && *src == type) || (trg && *trg == type);
}

void Planner::addMorph(SectionId sectionId,
    ::Anim::Duration duration,
    ::Anim::Duration delay,
    const std::optional<::Anim::Easing> &easing)
{
	if (animNeeded[sectionId]) {
		addElement(Morph::AbstractMorph::create(sectionId,
		               *source,
		               *target,
		               *actual),
		    getOptions(sectionId, duration, delay, easing));
	}
}

::Anim::Options Planner::getOptions(SectionId sectionId,
    ::Anim::Duration duration,
    ::Anim::Duration delay,
    const std::optional<::Anim::Easing> &easing)
{
	const auto &opt = options->get(sectionId);
	if (opt.duration) duration = *opt.duration;
	if (opt.delay) delay = *opt.delay - getBaseline();
	return ::Anim::Options{duration,
	    delay,
	    getEasing(sectionId, easing)};
}

::Anim::Easing Planner::getEasing(SectionId type,
    const std::optional<::Anim::Easing> &def) const
{
	auto res = def ? *def : defEasing();
	if (options->all.easing) res = *options->all.easing;
	if (options->get(type).easing) res = *options->get(type).easing;
	return res;
}

::Anim::Easing Planner::defEasing()
{
	return ::Anim::Easing{
	    &::Anim::EaseFunc::inOut<&::Anim::EaseFunc::cubic>};
}

}