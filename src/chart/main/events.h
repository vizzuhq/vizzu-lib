#ifndef EVENTS_H
#define EVENTS_H

#include <optional>

#include "base/anim/control.h"
#include "base/conv/auto_json.h"
#include "base/geom/line.h"
#include "base/geom/transformedrect.h"
#include "base/text/smartstring.h"
#include "base/util/eventdispatcher.h"
#include "chart/generator/marker.h"
#include "chart/options/channel.h"
#include "chart/rendering/renderedchart.h"

namespace Vizzu
{
class Chart;

class Events
{
public:
	explicit Events(Chart &chart);

	struct OnUpdateDetail
	{
		::Anim::Duration position;
		double progress;
	};

	struct OnUpdateEvent :
	    public Util::EventDispatcher::Params,
	    public OnUpdateDetail
	{
		explicit OnUpdateEvent(const ::Anim::Control &control) :
		    OnUpdateDetail{control.getPosition(),
		        control.getProgress()}
		{}

		void appendToJSON(Conv::JSONObj &json) const override
		{
			json("detail",
			    static_cast<const OnUpdateDetail &>(*this));
		}
	};

	struct OnDrawEvent : public Util::EventDispatcher::Params
	{
		explicit OnDrawEvent(const Util::EventTarget &target) :
		    Util::EventDispatcher::Params(&target)
		{}
	};

	struct OnRectDrawEvent : public OnDrawEvent
	{
		Draw::Rect rect;
		OnRectDrawEvent(const Util::EventTarget &target,
		    Draw::Rect rect) :
		    OnDrawEvent(target),
		    rect(rect)
		{}

		void appendToJSON(Conv::JSONObj &json) const override
		{
			json("detail", rect);
		}
	};

	struct OnLineDrawEvent : public OnDrawEvent
	{
		Draw::Line line;
		OnLineDrawEvent(const Util::EventTarget &target,
		    Draw::Line line) :
		    OnDrawEvent(target),
		    line(line)
		{}

		void appendToJSON(Conv::JSONObj &json) const override
		{
			json("detail", line);
		}
	};

	struct OnTextDrawDetail
	{
		Geom::TransformedRect rect;
		std::string_view text;
	};

	struct OnTextDrawEvent :
	    public OnDrawEvent,
	    public OnTextDrawDetail
	{
		OnTextDrawEvent(const Util::EventTarget &target,
		    Geom::TransformedRect rect,
		    std::string_view text) :
		    OnDrawEvent(target),
		    OnTextDrawDetail{rect, text}
		{}

		void appendToJSON(Conv::JSONObj &json) const override
		{
			json("detail",
			    static_cast<const OnTextDrawDetail &>(*this));
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

			explicit Element(std::string name) :
			    tagName(std::move(name))
			{}

			[[nodiscard]] std::string toJSON() const final
			{
				std::string res;
				{
					Conv::JSONObj jsonObj{res};
					appendToJSON(jsonObj);
				}
				return res;
			}

			virtual void appendToJSON(Conv::JSONObj &jsonObj) const
			{
				jsonObj("tagName", tagName);
			}
		};

		template <typename Parent> struct ChildOf : Element
		{
			Parent parent;

			template <typename... Args>
			explicit ChildOf(const std::string &name,
			    Args &&...args) :
			    Element(Parent::name() + "-" + name),
			    parent(args...)
			{}

			void appendToJSON(Conv::JSONObj &jsonObj) const override
			{
				Element::appendToJSON(jsonObj);
				jsonObj("parent", parent);
			}
		};

		template <class Base> struct Text : Base
		{
			std::string text;

			template <typename... Args>
			explicit Text(std::string text, Args &&...args) :
			    Base(args...),
			    text(std::move(text))
			{}

			void appendToJSON(Conv::JSONObj &jsonObj) const override
			{
				Base::appendToJSON(jsonObj);
				jsonObj("value", text);
			}
		};

		template <class Base> struct Label : Text<Base>
		{
			template <typename... Args>
			explicit Label(std::string text, Args &&...args) :
			    Text<Base>(std::move(text), "label", args...)
			{}
		};

		template <class Base> struct Title : Text<Base>
		{
			template <typename... Args>
			explicit Title(std::string text, Args &&...args) :
			    Text<Base>(std::move(text), "title", args...)
			{}
		};

		struct Legend : Element
		{
			static std::string name() { return "legend"; }

			Gen::ChannelId channel;

			explicit Legend(Gen::ChannelId channel) :
			    Element(name()),
			    channel(channel)
			{}

			void appendToJSON(Conv::JSONObj &jsonObj) const override
			{
				Element::appendToJSON(jsonObj);
				jsonObj("channel", channel);
			}
		};

		struct Axis : Element
		{
			static std::string name() { return "plot-axis"; }

			bool horizontal;

			explicit Axis(bool horizontal) :
			    Element(name()),
			    horizontal(horizontal)
			{}

			void appendToJSON(Conv::JSONObj &jsonObj) const override
			{
				Element::appendToJSON(jsonObj);
				jsonObj("id", (horizontal ? "x" : "y"));
			}
		};

		struct Marker : Element
		{
			static std::string name() { return "plot-marker"; }

			const Gen::Marker &marker;

			explicit Marker(const Gen::Marker &marker) :
			    Element(name()),
			    marker(marker)
			{}

			void appendToJSON(Conv::JSONObj &jsonObj) const override
			{
				Element::appendToJSON(jsonObj);
				marker.appendToJSON(jsonObj);
			}
		};

		struct MarkerGuide : ChildOf<Marker>
		{
			bool horizontal;

			MarkerGuide(const Gen::Marker &marker, bool horizontal) :
			    ChildOf<Marker>("guide", marker),
			    horizontal(horizontal)
			{}

			void appendToJSON(Conv::JSONObj &jsonObj) const override
			{
				ChildOf<Marker>::appendToJSON(jsonObj);
				jsonObj("id", (horizontal ? "x" : "y"));
			}
		};

		struct Root : Element
		{
			Root() : Element("root") {}
		};
		struct Plot : Element
		{
			Plot() : Element("plot") {}
		};
		struct Area : Element
		{
			Area() : Element("plot-area") {}
		};
		struct Logo : Element
		{
			Logo() : Element("logo") {}
		};

		using ChartTitle = Title<Element>;

		using MarkerLabel = Label<ChildOf<Marker>>;
		using LegendChild = ChildOf<Legend>;
		using LegendLabel = Label<ChildOf<Legend>>;
		using LegendTitle = Title<ChildOf<Legend>>;

		struct LegendMarker : LegendChild
		{
			explicit LegendMarker(Gen::ChannelId channel) :
			    LegendChild("marker", channel)
			{}
		};

		struct LegendBar : LegendChild
		{
			explicit LegendBar(Gen::ChannelId channel) :
			    LegendChild("bar", channel)
			{}
		};

		using AxisChild = ChildOf<Axis>;
		using AxisLabel = Label<ChildOf<Axis>>;
		using AxisTitle = Title<ChildOf<Axis>>;

		struct AxisGuide : AxisChild
		{
			explicit AxisGuide(bool horizontal) :
			    AxisChild("guide", horizontal)
			{}
		};

		struct AxisTick : AxisChild
		{
			explicit AxisTick(bool horizontal) :
			    AxisChild("tick", horizontal)
			{}
		};

		struct AxisInterlacing : AxisChild
		{
			explicit AxisInterlacing(bool horizontal) :
			    AxisChild("interlacing", horizontal)
			{}
		};
	};
};

}

#endif
