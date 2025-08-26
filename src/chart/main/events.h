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
class Events
{
public:
	explicit Events(Util::EventDispatcher &ed);

	struct OnUpdateDetail
	{
		::Anim::Duration position;
		[[maybe_unused]] double progress{};
	};

	struct OnUpdateEvent :
	    public Util::EventDispatcher::Params,
	    public OnUpdateDetail
	{
		explicit OnUpdateEvent(const ::Anim::Control &control) :
		    OnUpdateDetail{control.getPosition(),
		        control.getProgress()}
		{}

		void appendToJSON(Conv::JSON &json) const override
		{
			json.any<OnUpdateDetail>(*this);
		}
	};

	using OnDrawEvent = Util::EventDispatcher::Params;

	struct OnRectDrawEvent : public OnDrawEvent
	{
		Draw::Rect rect;
		OnRectDrawEvent(const Util::EventTarget &target,
		    const Draw::Rect &rect) :
		    OnDrawEvent(&target),
		    rect(rect)
		{}

		void appendToJSON(Conv::JSON &json) const override
		{
			json.any(rect);
		}
	};

	struct OnLineDrawEvent : public OnDrawEvent
	{
		Draw::Line line;
		OnLineDrawEvent(const Util::EventTarget &target,
		    const Draw::Line &line) :
		    OnDrawEvent(&target),
		    line(line)
		{}

		void appendToJSON(Conv::JSON &json) const override
		{
			json.any(line);
		}
	};

	struct OnTextDrawDetail
	{
		Geom::TransformedRect outerRect;
		[[maybe_unused]] Geom::Rect innerRect;
		[[maybe_unused]] double align{};
		std::string text;
	};

	struct OnTextDrawEvent :
	    public OnDrawEvent,
	    public OnTextDrawDetail
	{
		template <class... Ts>
		explicit OnTextDrawEvent(const Util::EventTarget &target,
		    Ts &&...ts) :
		    OnDrawEvent(&target),
		    OnTextDrawDetail{std::forward<Ts>(ts)...}
		{}

		void appendToJSON(Conv::JSON &json) const override
		{
			json.any<OnTextDrawDetail>(*this);
		}
	};

	struct DrawEvents
	{
		Util::EventDispatcher::event_ptr begin;
		Util::EventDispatcher::event_ptr background;
		Util::EventDispatcher::event_ptr title;
		Util::EventDispatcher::event_ptr subtitle;
		Util::EventDispatcher::event_ptr caption;
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

			explicit Element(std::string &&name) :
			    tagName(std::move(name))
			{}

			[[nodiscard]] std::string toJSON() const final
			{
				std::string res;
				appendToJSON(Conv::JSONObj{res});
				return res;
			}

			virtual void appendToJSON(Conv::JSONObj &&jsonObj) const
			{
				std::move(jsonObj)("tagName", tagName);
			}
		};

		template <typename Parent> struct ParentHolder
		{
			Parent parent;
		};

		template <typename Parent>
		struct ChildOf : ParentHolder<Parent>, Element
		{
			template <typename... Args>
			explicit ChildOf(const std::string &name,
			    Args &&...args) :
			    ParentHolder<Parent>{
			        Parent{std::forward<Args>(args)...}},
			    Element(this->parent.tagName + "-" + name)
			{}

			void appendToJSON(Conv::JSONObj &&jsonObj) const override
			{
				Element::appendToJSON(
				    std::move(jsonObj)("parent", this->parent));
			}
		};

		template <class Base> struct Text : Base
		{
			std::string text;

			template <typename... Args>
			explicit Text(std::string text, Args &&...args) :
			    Base(std::forward<Args>(args)...),
			    text(std::move(text))
			{}

			void appendToJSON(Conv::JSONObj &&jsonObj) const override
			{
				Base::appendToJSON(std::move(jsonObj)("value", text));
			}
		};

		struct LegendProperties
		{
			Gen::LegendId channel;
			double scrollTop{};
			double scrollHeight{};
		};

		struct Legend : Element
		{
			LegendProperties properties;

			explicit Legend(const LegendProperties &properties) :
			    Element("legend"),
			    properties(properties)
			{}

			void appendToJSON(Conv::JSONObj &&jsonObj) const override
			{
				Element::appendToJSON(
				    std::move(jsonObj.mergeObj(properties)));
			}
		};

		struct Axis : Element
		{
			Gen::AxisId axis;

			explicit Axis(Gen::AxisId axis) :
			    Element("plot-axis"),
			    axis(axis)
			{}

			void appendToJSON(Conv::JSONObj &&jsonObj) const override
			{
				Element::appendToJSON(std::move(jsonObj)("id", axis));
			}
		};

		struct Marker : Element
		{
			const Gen::Marker &marker;
			struct DataPosition
			{
				Geom::Point top;
				Geom::Point center;
			} position;

			explicit Marker(const Gen::Marker &marker,
			    const DataPosition &position) :
			    Element("plot-marker"),
			    marker(marker),
			    position(position)
			{}

			void appendToJSON(Conv::JSONObj &&jsonObj) const override
			{
				Element::appendToJSON(marker.appendToJSON(
				    std::move(jsonObj))("position", position));
			}
		};

		using LegendChild = ChildOf<Legend>;
		using AxisChild = ChildOf<Axis>;
		using MarkerChild = ChildOf<Marker>;

		struct MarkerGuide : MarkerChild
		{
			Gen::AxisId axis;

			MarkerGuide(const Gen::Marker &marker,
			    const Marker::DataPosition &position,
			    Gen::AxisId axis) :
			    MarkerChild("guide", marker, position),
			    axis(axis)
			{}

			void appendToJSON(Conv::JSONObj &&jsonObj) const override
			{
				MarkerChild::appendToJSON(
				    std::move(jsonObj)("id", axis));
			}
		};

		template <class Base> struct CategoryInfo : Base
		{
			std::string info;

			template <class... Args>
			explicit CategoryInfo(
			    const std::vector<Data::SliceIndex> &indices,
			    Args &&...args) :
			    Base(std::forward<Args>(args)...)
			{
				Conv::JSONObj cats{info};
				for (auto &&[name, value] : indices)
					cats.operator()<false>(name, value);
			}

			void appendToJSON(Conv::JSONObj &&jsonObj) const override
			{
				jsonObj.raw("categories", info);
				Base::appendToJSON(std::move(jsonObj));
			}
		};

		static auto axis(Gen::AxisId axis)
		{
			return std::make_unique<Axis>(axis);
		}

		static auto legend(const LegendProperties &properties)
		{
			return std::make_unique<Legend>(properties);
		}

		static auto marker(const Gen::Marker &marker,
		    const Marker::DataPosition &position)
		{
			return std::make_unique<Marker>(marker, position);
		}

		static auto markerGuide(const Gen::Marker &marker,
		    const Marker::DataPosition &position,
		    Gen::AxisId axis)
		{
			return std::make_unique<MarkerGuide>(marker,
			    position,
			    axis);
		}

		static auto root()
		{
			return std::make_unique<Element>("root");
		}

		static auto plot()
		{
			return std::make_unique<Element>("plot");
		}

		static auto area()
		{
			return std::make_unique<Element>("plot-area");
		}

		static auto logo()
		{
			return std::make_unique<Element>("logo");
		}

		static auto chartTitle(const std::string &title)
		{
			return std::make_unique<Text<Element>>(title, "title");
		}

		static auto chartSubtitle(const std::string &subtitle)
		{
			return std::make_unique<Text<Element>>(subtitle,
			    "subtitle");
		}

		static auto chartCaption(const std::string &caption)
		{
			return std::make_unique<Text<Element>>(caption,
			    "caption");
		}

		static auto markerLabel(const std::string &label,
		    const Gen::Marker &marker,
		    const Marker::DataPosition &position)
		{
			return std::make_unique<Text<MarkerChild>>(label,
			    "label",
			    marker,
			    position);
		}

		static auto dimLegendLabel(
		    const std::vector<Data::SliceIndex> &indices,
		    const std::string &categoryValue,
		    const LegendProperties &properties)
		{
			return std::make_unique<CategoryInfo<Text<LegendChild>>>(
			    indices,
			    categoryValue,
			    "label",
			    properties);
		}

		static auto measLegendLabel(const std::string &label,
		    const LegendProperties &properties)
		{
			return std::make_unique<Text<LegendChild>>(label,
			    "label",
			    properties);
		}

		static auto legendTitle(const std::string &title,
		    const LegendProperties &properties)
		{
			return std::make_unique<Text<LegendChild>>(title,
			    "title",
			    properties);
		}

		static auto legendMarker(
		    const std::vector<Data::SliceIndex> &indices,
		    const LegendProperties &properties)
		{
			return std::make_unique<CategoryInfo<LegendChild>>(
			    indices,
			    "marker",
			    properties);
		}

		static auto legendBar(const LegendProperties &properties)
		{
			return std::make_unique<LegendChild>("bar", properties);
		}

		static auto dimAxisLabel(
		    const std::vector<Data::SliceIndex> &indices,
		    const std::string &categoryValue,
		    Gen::AxisId axis)
		{
			return std::make_unique<CategoryInfo<Text<AxisChild>>>(
			    indices,
			    categoryValue,
			    "label",
			    axis);
		}

		static auto measAxisLabel(const std::string &label,
		    Gen::AxisId axis)
		{
			return std::make_unique<Text<AxisChild>>(label,
			    "label",
			    axis);
		}

		static auto axisTitle(const std::string &title,
		    Gen::AxisId axis)
		{
			return std::make_unique<Text<AxisChild>>(title,
			    "title",
			    axis);
		}

		static auto axisGuide(Gen::AxisId axis)
		{
			return std::make_unique<AxisChild>("guide", axis);
		}

		static auto axisTick(Gen::AxisId axis)
		{
			return std::make_unique<AxisChild>("tick", axis);
		}

		static auto axisInterlacing(Gen::AxisId axis)
		{
			return std::make_unique<AxisChild>("interlacing", axis);
		}
	};
};

}

#endif
