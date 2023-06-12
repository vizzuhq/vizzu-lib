#ifndef STYLESHEET_H
#define STYLESHEET_H

#include "base/style/sheet.h"
#include "chart/options/options.h"

#include "style.h"

namespace Vizzu
{
namespace Styles
{

class Sheet : public Style::Sheet<Chart>
{
public:
	typedef Style::Sheet<Chart> Base;
	using Base::Sheet;

	Chart getFullParams(const Diag::DiagramOptionsPtr &options,
	    const Geom::Size &size);

	static double baseFontSize(const Geom::Size &size, bool rounded);

private:
	using Base::getFullParams;

	const Diag::Options *options;

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
}

#endif
