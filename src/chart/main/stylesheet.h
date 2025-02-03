#ifndef STYLESHEET_H
#define STYLESHEET_H

#include "base/style/sheet.h"
#include "chart/options/options.h"

#include "style.h"

namespace Refl::Access
{
template <>
const std::map<std::string,
    Accessor<Vizzu::Styles::Chart, true>,
    std::less<>> &
getAccessors<Vizzu::Styles::Chart, true>();
}

namespace Vizzu::Gen
{
class Plot;
}

namespace Vizzu::Styles
{

class Sheet : public Style::Sheet<Chart>
{
public:
	using Base = Style::Sheet<Chart>;

	Sheet(Chart &&defaultParams, Chart &activeParams) :
	    Base(std::move(defaultParams), activeParams)
	{
		this->defaultParams.setup();
	}

	Chart getFullParams(const Gen::PlotOptionsPtr &options,
	    const Geom::Size &size);

	static double baseFontSize(const Geom::Size &size, bool rounded);

	static void setAfterStyles(Gen::Plot &plot,
	    const Geom::Size &size);

private:
	using Base::getFullParams;

	const Gen::Options *options{};

	void calcDefaults(const Geom::Size &size);

	void setBaseFontSize(const Geom::Size &size);
	void setPlot();
	void setAxis();
	void setAxisLabels();
	void setAxisTitle();
	void setMarkers();
	void setMarkerLabels();
	void setData();
	static double nominalSize(const Geom::Size &size);
};

}

#endif
