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
		[[nodiscard]] std::string dataToJson() const override
		{
			return R"("position":")" + std::string(position)
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
		[[nodiscard]] std::string dataToJson() const override
		{
			return OnDrawParam::dataToJson()
			     + +"\"rect\":" + rect.toJSON();
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
		[[nodiscard]] std::string dataToJson() const override
		{
			return OnDrawParam::dataToJson()
			     + +"\"line\":" + line.toJSON();
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

		[[nodiscard]] std::string dataToJson() const override
		{
			return OnDrawParam::dataToJson()
			     + "\"rect\":" + rect.toJSON()
			     + ","
			       "\"text\": \""
			     + std::string(text) + "\"";
		}
	};

	struct Draw
	{
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

	struct Targets
	{
		Targets();
		Util::EventTarget root;
		Util::EventTarget title;
		Util::EventTarget legend;
		Util::EventTarget legendTitle;
		Util::EventTarget legendLabel;
		Util::EventTarget legendMarker;
		Util::EventTarget legendBar;
		Util::EventTarget plot;
		Util::EventTarget area;
		Util::EventTarget xAxis;
		Util::EventTarget yAxis;
		Util::EventTarget xTitle;
		Util::EventTarget yTitle;
		Util::EventTarget xLabel;
		Util::EventTarget yLabel;
		Util::EventTarget xInterlacing;
		Util::EventTarget yInterlacing;
		Util::EventTarget xGuide;
		Util::EventTarget yGuide;
		Util::EventTarget xTick;
		Util::EventTarget yTick;
		Util::EventTarget marker;
		Util::EventTarget markerLabel;
		Util::EventTarget markerXGuide;
		Util::EventTarget markerYGuide;
		Util::EventTarget logo;
	} targets;

protected:
	class Chart &chart;
};

}

#endif
