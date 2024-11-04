#ifndef CHART_ANIM_PLANNER
#define CHART_ANIM_PLANNER

#include <functional>
#include <memory>
#include <vector>

#include "base/anim/control.h"
#include "base/anim/easingfunc.h"
#include "base/anim/group.h"
#include "chart/generator/marker.h"
#include "chart/generator/plotptr.h"

#include "options.h"

namespace Vizzu::Anim
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
	const Gen::Plot *source{};
	const Gen::Plot *target{};
	Gen::Plot *actual{};
	const Options::Keyframe *options{};
	using AnimNeeded = Refl::EnumArray<SectionId, bool>;

	AnimNeeded animNeeded{};

	void reset();
	void calcNeeded();

	void addMorph(SectionId sectionId,
	    ::Anim::Duration duration,
	    ::Anim::Duration delay = ::Anim::Duration(0),
	    const std::optional<::Anim::Easing> &easing = std::nullopt);

	bool anyMarker(bool (
	    *compare)(const Gen::Marker &, const Gen::Marker &)) const;

	[[nodiscard]] bool positionMorphNeeded() const;
	[[nodiscard]] bool verticalBeforeHorizontal() const;
	static size_t dimensionCount(const Gen::Plot *plot,
	    Gen::AxisId type);

	::Anim::Options getOptions(SectionId sectionId,
	    ::Anim::Duration duration,
	    ::Anim::Duration delay = ::Anim::Duration(0),
	    const std::optional<::Anim::Easing> &easing = std::nullopt);

	[[nodiscard]] ::Anim::Easing getEasing(SectionId type,
	    const std::optional<::Anim::Easing> &def =
	        std::nullopt) const;
	static ::Anim::Easing defEasing();

	[[nodiscard]] bool needColor() const;
	[[nodiscard]] bool needHorizontal() const;
	[[nodiscard]] bool needVertical() const;

	void reTime();
};

}

#endif
