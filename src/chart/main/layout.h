#ifndef CHART_LAYOUT_H
#define CHART_LAYOUT_H

#include "base/geom/rect.h"
#include "chart/options/options.h"

namespace Vizzu
{
namespace Styles
{
struct Chart;
struct Logo;
}

class Layout
{
public:
	Layout() = default;

	Geom::Rect boundary;
	Geom::Rect title;
	Geom::Rect subtitle;
	Geom::Rect caption;
	Geom::Rect legend;
	Geom::Rect plot;
	Geom::Rect plotArea;
	Geom::Rect logo;

	void setBoundary(const Geom::Rect &boundary,
	    const Styles::Chart &style,
	    const Gen::PlotOptionsPtr &options);

private:
	void setLogoBoundary(const Styles::Logo &logoStyle);
};

}

#endif
