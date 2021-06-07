#ifndef CHART_RENDERING_DRAWLABEL_H
#define CHART_RENDERING_DRAWLABEL_H

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"
#include "chart/main/style.h"

namespace Vizzu
{
namespace Draw
{

class drawLabel
{
public:
	drawLabel(const Geom::Rect &rect,
	    const std::string &text,
	    const Styles::Label &style,
	    Gfx::ICanvas &canvas,
	    bool setColor = true,
		double alpha = 1.0);

	static double getHeight(const Styles::Label &style,
	    Gfx::ICanvas &canvas);

private:
	Geom::Rect contentRect;
	std::string text;
	const Styles::Label &style;
	Gfx::ICanvas &canvas;
	bool overflows;
	Geom::Rect alignText(const Geom::Size &textSize);
	Geom::Size getTextSize();
};

}
}

#endif
