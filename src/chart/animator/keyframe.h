#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "chart/generator/plotptr.h"

#include "options.h"
#include "planner.h"

namespace Vizzu::Anim
{

class Keyframe : public Planner
{
public:
	Keyframe(Gen::PlotPtr src,
	    const Gen::PlotPtr &trg,
	    const Data::DataTable &dataTable,
	    const Options::Keyframe *options,
	    bool isInstant);

	[[nodiscard]] std::shared_ptr<void> data() const final
	{
		return actual;
	}

private:
	Options::Keyframe options;
	Gen::PlotPtr source;
	Gen::PlotPtr target;
	Gen::PlotPtr actual;
	Gen::PlotPtr targetCopy;

	void init(const Gen::PlotPtr &plot,
	    const Data::DataTable &dataTable);
	void prepareActual();
	void prepareActualMarkersInfo();
	void addMissingMarkers(const Gen::PlotPtr &source,
	    const Gen::PlotPtr &target);
	static void mergeMarkerCellInfo(const Gen::PlotPtr &source,
	    const Gen::PlotPtr &target);
	void copyTarget();
};

}

#endif
