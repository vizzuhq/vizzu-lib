#ifndef EVENTS_H
#define EVENTS_H

#include <optional>

#include "base/anim/control.h"
#include "base/conv/auto_json.h"
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
			     + "\"rect\":" + Conv::toJSON(rect);
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
			     + "\"line\":" + Conv::toJSON(line);
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
			     + "\"rect\":" + Conv::toJSON(rect)
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
		struct Target : Util::EventTarget
		{
			std::string name;

			Target(const std::string &name, const Util::EventTarget *parent) :
			    Util::EventTarget(parent),
				name(name)
			{}

			[[nodiscard]] std::string toJson() const override
			{
				return "\"name\":\"" + name + "\"";
			}
		};

		Targets();
		Target root;
		Target title;
		Target legend;
		Target legendTitle;
		Target legendLabel;
		Target legendMarker;
		Target legendBar;
		Target plot;
		Target area;
		Target xAxis;
		Target yAxis;
		Target xTitle;
		Target yTitle;
		Target xLabel;
		Target yLabel;
		Target xInterlacing;
		Target yInterlacing;
		Target xGuide;
		Target yGuide;
		Target xTick;
		Target yTick;
		Target marker;
		Target markerLabel;
		Target markerXGuide;
		Target markerYGuide;
		Target logo;
	} targets;

protected:
	class Chart &chart;
};

}

#endif
