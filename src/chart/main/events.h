#ifndef EVENTS_H
#define EVENTS_H

#include <optional>

#include "base/anim/control.h"
#include "base/conv/auto_json.h"
#include "base/geom/line.h"
#include "base/geom/transformedrect.h"
#include "base/util/eventdispatcher.h"
#include "base/text/smartstring.h"
#include "chart/generator/marker.h"
#include "chart/options/channel.h"
#include "chart/rendering/renderedchart.h"

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
		Draw::Rect rect;
		OnRectDrawParam(const Util::EventTarget &target,
		    Draw::Rect rect) :
		    OnDrawParam(target),
		    rect(rect)
		{}
		[[nodiscard]] std::string dataToJson() const override
		{
			return OnDrawParam::dataToJson()
			     + "\"rect\":" + Conv::toJSON(rect.rect)
				 + ","
				   "\"relative\":" + Conv::toJSON(rect.usesBaseTransform);
		}
	};

	struct OnLineDrawParam : public OnDrawParam
	{
		Draw::Line line;
		OnLineDrawParam(const Util::EventTarget &target,
		    Draw::Line line) :
		    OnDrawParam(target),
		    line(line)
		{}
		[[nodiscard]] std::string dataToJson() const override
		{
			return OnDrawParam::dataToJson()
			     + "\"line\":" + Conv::toJSON(line.line)
			     + ","
			       "\"relative\":" + Conv::toJSON(line.usesBaseTransform);
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

	struct DrawEvents
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
		struct Element : Util::EventTarget
		{
			std::string tagName;

			Element(const std::string &name) : tagName(name) {}

			[[nodiscard]] std::string toJson() const override
			{
				return "\"tagName\":\"" + tagName + "\"";
			}
		};

		template <typename Parent>
		struct ChildOf : Element
		{
			Parent parent;

			template <typename ... Args>
			ChildOf(const std::string &name, Args &&...args) :
			    Element(Parent::name()+ "-" + name),
				parent(args...)
			{}

			[[nodiscard]] std::string toJson() const override
			{
				return Element::toJson()
					+ ",\"parent\":{" + parent.toJson() + "}";
			}
		};

		template <class Base>
		struct Text : Base
		{
			std::string text;

			template <typename ... Args>
			Text(const std::string &text, Args &&...args) :
			    Base(args...),
				text(text)
			{}

			[[nodiscard]] std::string toJson() const override
			{
				return Base::toJson()
				     + ",\"value\":\"" 
					 + ::Text::SmartString::escape(text) + "\"";
			}
		};

		template <class Base>
		struct Label : Text<Base>
		{
			template <typename ... Args>
			Label(const std::string &text, Args &&...args) 
				: Text<Base>(text, "label", args...) {}
		};

		template <class Base>
		struct Title : Text<Base>
		{
			template <typename ... Args>
			Title(const std::string &text, Args &&...args) 
				: Text<Base>(text, "title", args...) {}
		};

		struct Legend : Element
		{
			static std::string name() { return "legend"; }

			Gen::ChannelId channel;

			Legend(Gen::ChannelId channel) : Element(name()), channel(channel) 
			{}

			[[nodiscard]] std::string toJson() const override
			{
				return Element::toJson()
				     + ",\"channel\":\"" + Conv::toString(channel) + "\"";
			}
		};

		struct Axis : Element
		{
			static std::string name() { return "plot-axis"; }

			bool horizontal;

			Axis(bool horizontal) :
			    Element(name()),
				horizontal(horizontal)
			{}

			[[nodiscard]] std::string toJson() const override
			{
				return Element::toJson()
				     + ",\"id\":" + (horizontal ? "\"x\"" : "\"y\"");
			}
		};

		struct Marker : Element
		{
			static std::string name() { return "plot-marker"; }

			const Gen::Marker &marker;

			Marker(const Gen::Marker &marker) :
			    Element(name()),
				marker(marker)
			{}

			[[nodiscard]] std::string toJson() const override
			{
				return Element::toJson()
				     + ","
				     + marker.toJson();
			}
		};

		struct MarkerGuide : ChildOf<Marker>
		{
			bool horizontal;

			MarkerGuide(const Gen::Marker &marker, bool horizontal) :
			    ChildOf<Marker>("guide", marker),
				horizontal(horizontal)
			{}

			[[nodiscard]] std::string toJson() const override
			{
				return ChildOf<Marker>::toJson()
				     + ",\"id\":" + (horizontal ? "\"x\"" : "\"y\"");
			}
		};

		struct Root : Element { Root() : Element("root") {} };
		struct Plot : Element { Plot() : Element("plot") {} };
		struct Area : Element { Area() : Element("plot-area") {} };
		struct Logo : Element { Logo() : Element("logo") {} };

		using ChartTitle = Title<Element>;

		using MarkerLabel = Label<ChildOf<Marker>>;
		using LegendChild = ChildOf<Legend>;
		using LegendLabel = Label<ChildOf<Legend>>;
		using LegendTitle = Title<ChildOf<Legend>>;

		struct LegendMarker: LegendChild { 
			LegendMarker(Gen::ChannelId channel) 
			: LegendChild("marker", channel) {} 
		};

		struct LegendBar: LegendChild { 
			LegendBar(Gen::ChannelId channel) 
			: LegendChild("bar", channel) {} 
		};

		using AxisChild = ChildOf<Axis>;
		using AxisLabel = Label<ChildOf<Axis>>;
		using AxisTitle = Title<ChildOf<Axis>>;
		
		struct AxisGuide: AxisChild { 
			AxisGuide(bool horizontal) : AxisChild("guide", horizontal) {} 
		};

		struct AxisTick: AxisChild { 
			AxisTick(bool horizontal) : AxisChild("tick", horizontal) {} 
		};

		struct AxisInterlacing: AxisChild { 
			AxisInterlacing(bool horizontal) 
			: AxisChild("interlacing", horizontal) {} 
		};

	};

protected:
	class Chart &chart;
};

}

#endif
