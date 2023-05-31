#ifndef EVENTS_H
#define EVENTS_H

#include "base/io/log.h"
#include "base/geom/line.h"
#include "base/geom/rect.h"
#include "base/anim/control.h"
#include "base/util/eventdispatcher.h"

namespace Vizzu
{

class Events {
public:
	Events(class Chart &chart);

	struct OnUpdateParam : public Util::EventDispatcher::Params
	{
		::Anim::Duration position;
		double progress;
		OnUpdateParam(const ::Anim::Control &control) {
			position = control.getPosition();
			progress = control.getProgress();
		}
		std::string dataToJson() const override {
			return "\"position\":\"" + std::string(position) + "\","
				"\"progress\":" + std::to_string(progress);
		}
	};

	struct OnDrawParam : public Util::EventDispatcher::Params
	{
		size_t markerIndex;
		const char *elementName;
		OnDrawParam(const char *elementName, size_t markerIndex = -1) : 
			markerIndex(markerIndex), elementName(elementName)
		{}
		std::string dataToJson() const override {
			return 
				(elementName ? "\"element\":\"" + std::string(elementName) + "\"," : "") +
				(markerIndex >= 0 
					? "\"markerId\":" + std::to_string(markerIndex) + "," 
					: std::string());
		}
	};

	struct OnRectDrawParam : public OnDrawParam
	{
		size_t markerIndex;
		Geom::Rect rect;
		OnRectDrawParam(const char *elementName, size_t markerIndex = -1) 
		: OnDrawParam(elementName, markerIndex)
		{}
		OnRectDrawParam(const char *elementName, Geom::Rect rect, size_t markerIndex = -1) 
		: OnDrawParam(elementName, markerIndex), rect(rect) 
		{}
		std::string dataToJson() const override {
			return OnDrawParam::dataToJson() +
				+ "\"rect\":" + std::string(rect);
		}
	};

	struct OnLineDrawParam : public OnDrawParam
	{
		Geom::Line line;
		OnLineDrawParam(const char *elementName, Geom::Line line, size_t markerIndex = -1)
		: OnDrawParam(elementName, markerIndex), line(line) 
		{}
		std::string dataToJson() const override {
			return OnDrawParam::dataToJson() +
				+ "\"line\":" + std::string(line);
		}
	};

	struct OnTextDrawParam : public OnDrawParam
	{
		Geom::Rect rect;
		std::string_view text;
		
		OnTextDrawParam(const char *elementName) : 
			OnDrawParam(elementName)
		{}

		OnTextDrawParam(const char *elementName, Geom::Rect rect, std::string_view text) : 
			OnDrawParam(elementName), rect(rect), text(text)
		{}

		std::string dataToJson() const override {
			return OnDrawParam::dataToJson() +
				"\"rect\":" + std::string(rect) + ","
				"\"text\": \"" + std::string(text) + "\"";
		}
	};

	struct Draw {
		Util::EventDispatcher::event_ptr begin;
		Util::EventDispatcher::event_ptr background;
		Util::EventDispatcher::event_ptr title;
		Util::EventDispatcher::event_ptr logo;
		struct Legend {
			Util::EventDispatcher::event_ptr background;
			Util::EventDispatcher::event_ptr title;
			Util::EventDispatcher::event_ptr label;
			Util::EventDispatcher::event_ptr marker;
			Util::EventDispatcher::event_ptr bar;
		} legend;
		struct Plot {
			Util::EventDispatcher::event_ptr background;
			Util::EventDispatcher::event_ptr area;
			struct Marker {
				Util::EventDispatcher::event_ptr base;
				Util::EventDispatcher::event_ptr label;
				Util::EventDispatcher::event_ptr guide;
			} marker;
			struct Axis {
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
	struct Animation {
		Util::EventDispatcher::event_ptr begin;
		Util::EventDispatcher::event_ptr update;
		Util::EventDispatcher::event_ptr complete;
	} animation;

protected:
    class Chart& chart;
};

}

#endif
