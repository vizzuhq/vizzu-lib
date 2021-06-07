#ifndef CHART_ANIM_PLANNER
#define CHART_ANIM_PLANNER

#include <functional>
#include <memory>
#include <vector>

#include "base/anim/control.h"
#include "base/anim/group.h"
#include "chart/generator/diagram.h"

namespace Vizzu
{
namespace Anim
{

class Planner : public ::Anim::Group
{
public:
	Planner() = default;

protected:

	void createPlan(const Diag::Diagram &source,
	    const Diag::Diagram &target,
	    Diag::Diagram &actual);

private:
	const Diag::Diagram *source;
	const Diag::Diagram *target;
	Diag::Diagram *actual;

	template <typename M>
	void addMorph(std::chrono::nanoseconds duration,
	    std::chrono::nanoseconds delay = std::chrono::nanoseconds(0));

	bool anyMarker(const std::function<bool(const Diag::DiagramItem &,
	        const Diag::DiagramItem &)> &compare) const;

	bool positionMorphNeeded() const;
	bool verticalBeforeHorizontal() const;
	size_t discreteCount(const Diag::Diagram *diagram,
	    Diag::Scale::Type type) const;

	bool needColor() const;
	bool needHorizontal() const;
	bool needVertical() const;
};
}
}

#endif
