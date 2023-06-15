#ifndef CHART_ANIM_PLANNER
#define CHART_ANIM_PLANNER

#include <functional>
#include <memory>
#include <vector>

#include "base/anim/control.h"
#include "base/anim/easingfunc.h"
#include "base/anim/group.h"
#include "base/util/templates.h"
#include "chart/generator/plot.h"

#include "options.h"

namespace Vizzu
{
namespace Anim
{

class Planner : public ::Anim::Group
{
public:
	Planner() = default;

protected:
	void createPlan(const Gen::Plot &source,
	    const Gen::Plot &target,
	    Gen::Plot &actual,
	    const Options::Keyframe &options);

private:
	const Gen::Plot *source;
	const Gen::Plot *target;
	Gen::Plot *actual;
	const Options::Keyframe *options;
	typedef Refl::EnumArray<SectionId, bool> AnimNeeded;

	AnimNeeded animNeeded;

	void reset();
	void calcNeeded();

	void addMorph(SectionId sectionId,
	    ::Anim::Duration duration,
	    ::Anim::Duration delay = ::Anim::Duration(0),
	    std::optional<::Anim::Easing> easing = std::nullopt);

	bool anyMarker(const std::function<bool(const Gen::Marker &,
	        const Gen::Marker &)> &compare) const;

	bool positionMorphNeeded() const;
	bool verticalBeforeHorizontal() const;
	size_t dimensionCount(const Gen::Plot *plot,
	    Gen::ChannelId type) const;

	bool isAnyLegend(Gen::ChannelId type) const;

	::Anim::Options getOptions(SectionId sectionId,
	    ::Anim::Duration duration,
	    ::Anim::Duration delay = ::Anim::Duration(0),
	    std::optional<::Anim::Easing> easing = std::nullopt);

	::Anim::Easing getEasing(SectionId type,
	    const std::optional<::Anim::Easing> &def =
	        std::nullopt) const;
	::Anim::Easing defEasing() const;

	bool needColor() const;
	bool needHorizontal() const;
	bool needVertical() const;
	::Anim::Options defOptions(double wholeDuration,
	    double delayFactor = 0,
	    double durationFactor = 1,
	    const ::Anim::Easing &easing = ::Anim::Easing(
	        &::Anim::EaseFunc::inOut<&::Anim::EaseFunc::cubic>))
	    const;

	void reTime();
};

}
}

#endif
