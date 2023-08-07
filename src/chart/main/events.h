#ifndef EVENTS_H
#define EVENTS_H

#include <optional>

#include "base/anim/control.h"
#include "base/geom/line.h"
#include "base/geom/rect.h"
#include "base/conv/auto_json.h"
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
		std::optional<int> markerIndex;
		const char *elementName;
		OnDrawParam(const char *elementName,
		    std::optional<int> markerIndex = {}) :
		    markerIndex(markerIndex),
		    elementName(elementName)
		{}
		[[nodiscard]] std::string dataToJson() const override
		{
			return (elementName
			               ? R"("element":")"
			                     + std::string(elementName) + "\","
			               : "")
			     + (markerIndex.has_value()
			             ? "\"markerId\":"
			                   + std::to_string(*markerIndex) + ","
			             : std::string());
		}
	};

	struct OnRectDrawParam : public OnDrawParam
	{
		Geom::Rect rect;
		OnRectDrawParam(const char *elementName,
		    std::optional<int> markerIndex = {}) :
		    OnDrawParam(elementName, markerIndex)
		{}
		OnRectDrawParam(const char *elementName,
		    Geom::Rect rect,
		    std::optional<int> markerIndex = {}) :
		    OnDrawParam(elementName, markerIndex),
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
		OnLineDrawParam(const char *elementName,
		    Geom::Line line,
		    std::optional<int> markerIndex = {}) :
		    OnDrawParam(elementName, markerIndex),
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
		Geom::Rect rect;
		std::string_view text;

		OnTextDrawParam(const char *elementName) :
		    OnDrawParam(elementName)
		{}

		OnTextDrawParam(const char *elementName,
		    Geom::Rect rect,
		    std::string_view text) :
		    OnDrawParam(elementName),
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

protected:
	class Chart &chart;
};

}

#endif
