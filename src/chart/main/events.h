#ifndef EVENTS_H
#define EVENTS_H

#include <optional>

#include "base/anim/control.h"
#include "base/geom/line.h"
#include "base/geom/transformedrect.h"
#include "base/util/eventdispatcher.h"

namespace Vizzu
{

class Events
{
public:
	Events(class Chart &chart);

	struct OnUpdateParam : public Util::EventDispatcher::Params
	{
		::Anim::Duration position;
		double progress;
		OnUpdateParam(const ::Anim::Control &control)
		{
			position = control.getPosition();
			progress = control.getProgress();
		}
		std::string dataToJson() const override
		{
			return "\"position\":\"" + std::string(position)
			     + "\","
			       "\"progress\":"
			     + std::to_string(progress);
		}
	};

	struct OnDrawParam : public Util::EventDispatcher::Params
	{
		OnDrawParam(const Util::EventTarget &target) 
			: Util::EventDispatcher::Params(&target) 
		{}
	};

	struct OnRectDrawParam : public OnDrawParam
	{
		Geom::Rect rect;
		OnRectDrawParam(const Util::EventTarget &target,
		    Geom::Rect rect) :
		    OnDrawParam(target),
		    rect(rect)
		{}
		std::string dataToJson() const override
		{
			return OnDrawParam::dataToJson()
			     + +"\"rect\":" + std::string(rect);
		}
	};

	struct OnLineDrawParam : public OnDrawParam
	{
		Geom::Line line;
		OnLineDrawParam(const Util::EventTarget &target,
		    Geom::Line line) :
		    OnDrawParam(target),
		    line(line)
		{}
		std::string dataToJson() const override
		{
			return OnDrawParam::dataToJson()
			     + +"\"line\":" + std::string(line);
		}
	};

	struct OnTextDrawParam : public OnDrawParam
	{
		Geom::TransformedRect rect;
		std::string_view text;

		OnTextDrawParam(const Util::EventTarget &target,
		    Geom::TransformedRect rect,
		    std::string_view text) :
		    OnDrawParam(target),
		    rect(rect),
		    text(text)
		{}

		std::string dataToJson() const override
		{
			return OnDrawParam::dataToJson()
			     + "\"rect\":" + std::string(rect)
			     + ","
			       "\"text\": \""
			     + std::string(text) + "\"";
		}
	};

	struct Draw
	{
		Draw();
		Util::EventTarget rootElement;
		Util::EventTarget titleElement;
		Util::EventTarget legendElement;
		Util::EventTarget legendTitleElement;
		Util::EventTarget legendLabelElement;
		Util::EventTarget legendBarElement;
		Util::EventTarget plotElement;
		Util::EventTarget areaElement;
		Util::EventTarget xAxisElement;
		Util::EventTarget yAxisElement;
		Util::EventTarget xTitleElement;
		Util::EventTarget yTitleElement;
		Util::EventTarget xInterlacingElement;
		Util::EventTarget yInterlacingElement;
		Util::EventTarget xGuideElement;
		Util::EventTarget yGuideElement;
		Util::EventTarget xTickElement;
		Util::EventTarget yTickElement;
		Util::EventDispatcher::event_ptr begin;
		Util::EventDispatcher::event_ptr background;
		Util::EventDispatcher::event_ptr title;
		Util::EventDispatcher::event_ptr logo;
		struct Legend
		{
			Util::EventDispatcher::event_ptr background;
			Util::EventDispatcher::event_ptr title;
			Util::EventDispatcher::event_ptr label;
			Util::EventDispatcher::event_ptr marker;
			Util::EventDispatcher::event_ptr bar;
		} legend;
		struct Plot
		{
			Util::EventDispatcher::event_ptr background;
			Util::EventDispatcher::event_ptr area;
			struct Marker
			{
				Util::EventDispatcher::event_ptr base;
				Util::EventDispatcher::event_ptr label;
				Util::EventDispatcher::event_ptr guide;
			} marker;
			struct Axis
			{
				Util::EventDispatcher::event_ptr base;
				Util::EventDispatcher::event_ptr title;
				Util::EventDispatcher::event_ptr label;
				Util::EventDispatcher::event_ptr tick;
				Util::EventDispatcher::event_ptr guide;
				Util::EventDispatcher::event_ptr interlacing;
			} axis;
		} plot;
		Util::EventDispatcher::event_ptr complete;
	} draw;
	struct Animation
	{
		Util::EventDispatcher::event_ptr begin;
		Util::EventDispatcher::event_ptr update;
		Util::EventDispatcher::event_ptr complete;
	} animation;

protected:
	class Chart &chart;
};

}

#endif
