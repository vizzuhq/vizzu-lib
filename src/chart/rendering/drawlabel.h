#ifndef CHART_RENDERING_DRAWLABEL_H
#define CHART_RENDERING_DRAWLABEL_H

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/style.h"

namespace Vizzu
{
namespace Draw
{

class drawLabel
{
public:
	struct OnTextDrawParam : public Util::EventDispatcher::Params
	{
		size_t markerIndex;
		Geom::Rect rect;
		std::string_view text;
		
		OnTextDrawParam() : 
			markerIndex(-1)
		{}

		OnTextDrawParam(Geom::Rect rect, std::string_view text) : 
			markerIndex(-1), rect(rect), text(text)
		{}

		std::string dataToJson() const override {
			return 
				(markerIndex >= 0 
					? "\"markerId\":" + std::to_string(markerIndex) + "," 
					: std::string()) +
				"\"rect\":" + std::string(rect) + ","
				"\"text\": \"" + std::string(text) + "\"";
		}
	};

	drawLabel(const Geom::Rect &rect,
	    const std::string &text,
	    const Styles::Label &style,
		const Util::EventDispatcher::event_ptr &onDraw,
	    Gfx::ICanvas &canvas,
	    bool setColor = true,
		double alpha = 1.0);

	static double getHeight(const Styles::Label &style,
	    Gfx::ICanvas &canvas);

private:
	Geom::Rect contentRect;
	std::string text;
	const Styles::Label &style;
	const Util::EventDispatcher::event_ptr &onDraw;
	Gfx::ICanvas &canvas;
	bool overflows;
	Geom::Rect alignText(const Geom::Size &textSize);
	Geom::Size getTextSize();
};

}
}

#endif
