
#include <chart/rendering/painter/painter.h>

#include "../util/test.h"
#include "chart/ui/chart.h"

using test::operator""_suite;
using test::assert;
using test::check;
using test::skip;
using test::operator""_is_true;
using test::operator""_is_false;

Geom::Size Gfx::ICanvas::textBoundary(const Font &font,
    const std::string &text)
{
	return {static_cast<double>(text.size()) * font.size / 2.0,
	    font.size};
}

struct MyCanvas final : Gfx::ICanvas, Vizzu::Draw::Painter
{
	MyCanvas() noexcept = default;
	~MyCanvas() final = default;
	void setClipRect(const Geom::Rect &) final {}
	void setClipCircle(const Geom::Circle &) final {}
	void setClipPolygon() final {}
	void setBrushColor(const Gfx::Color &) final {}
	void setLineColor(const Gfx::Color &) final {}
	void setTextColor(const Gfx::Color &) final {}
	void setLineWidth(double) final {}
	void setFont(const Gfx::Font &) final {}
	void transform(const Geom::AffineTransform &) final {}
	void save() final {}
	void restore() final {}
	void beginDropShadow() final {}
	void setDropShadowBlur(double) final {}
	void setDropShadowColor(const Gfx::Color &) final {}
	void setDropShadowOffset(const Geom::Point &) final {}
	void endDropShadow() final {}
	void beginPolygon() final {}
	void addPoint(const Geom::Point &) final {}
	void addBezier(const Geom::Point &,
	    const Geom::Point &,
	    const Geom::Point &) final
	{}
	void endPolygon() final {}
	void rectangle(const Geom::Rect &) final {}
	void circle(const Geom::Circle &) final {}
	void line(const Geom::Line &) final {}
	void text(const Geom::Rect &, const std::string &) final {}
	void setBrushGradient(const Geom::Line &,
	    const Gfx::ColorGradient &) final
	{}
	void frameBegin() final {}
	void frameEnd() final {}
	void *getPainter() final { return static_cast<Painter *>(this); }
	ICanvas &getCanvas() final { return *this; }
};

struct chart_setup
{
	std::vector<std::pair<Vizzu::Gen::ChannelId, const char *>>
	    series;
	bool is_emscripten{[]
	    {
		    bool is_emscripten{
#ifdef __EMSCRIPTEN__
		        true
#endif
		    };

		    skip->*is_emscripten == "Emscripten build"_is_false;
		    return is_emscripten;
	    }()};
	Vizzu::Chart chart{};

	explicit(false) operator Vizzu::Chart &()
	{
		auto &table = chart.getTable();
		table.addColumn("Dim5",
		    {{"A", "B", "C", "D", "E"}},
		    {{0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4}});
		table.addColumn("Dim4",
		    {{"a", "b", "c", "d"}},
		    {{0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}});
		table.addColumn("Dim3",
		    {{"a", "b", "c"}},
		    {{0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 1, 0, 2, 2, 1, 0}});
		table.addColumn("Meas1",
		    "",
		    {{1, 2, 4, 3, 3, 4, 2, 1, 4, 3, 1, 2, 2, 1, 3, 4}});
		table.addColumn("Meas2",
		    "",
		    {{0, -1, 5, 6, 6, 5, -1, 0, 5, 6, 0, -1, -1, 0, 6, -5}});
		auto &&setter = chart.getSetter();
		for (auto &&[ch, name] : series) setter.addSeries(ch, name);
		chart.setBoundRect(Geom::Rect(Geom::Point{}, {{640, 480}}));
		return chart;
	}
};

struct event_as
{
	std::string json;
	const Util::EventTarget *target;
	std::variant<std::monostate, Vizzu::Draw::Rect, Vizzu::Draw::Line>
	    drawn;
};

std::multimap<std::string, event_as, std::less<>> get_events(
    Vizzu::Chart &chart)
{
	chart.getAnimOptions().control.position = 1.0;

	bool ends{};
	chart.setKeyframe();
	chart.animate(
	    [&ends](bool b)
	    {
		    ends = b;
	    });

	std::multimap<std::string, event_as, std::less<>> events;

	static std::vector<std::string> eventNames = []
	{
		std::vector<std::string> names;
		Refl::visit<Vizzu::Events::DrawEvents>(
		    [&names]<class T>(
		        Util::EventDispatcher::event_ptr &(*)(const T &),
		        const std::initializer_list<std::string_view> &sv =
		            {})
		    {
			    std::span s{sv};
			    if (s.back() == "begin" || s.back() == "complete") {
				    return names.emplace_back(
				        "draw-" + std::string{s.back()});
			    }

			    if (s.back() == "base") s = {s.begin(), s.end() - 1};

			    std::string name;
			    for (auto name_part : s) {
				    name += name_part;
				    name += '-';
			    }
			    name += "draw";
			    return names.emplace_back(name);
		    });
		return names;
	}();

	auto line =
	    chart.getOptions().geometry == Vizzu::Gen::ShapeType::line;
	auto event_handler = [&events, &line](
	                         Util::EventDispatcher::Params &params)
	{
		auto marker = params.eventName == "plot-marker-draw";
		if ((marker && line)
		    || params.eventName == "plot-axis-draw") {
			events.emplace(std::piecewise_construct,
			    std::tuple{params.eventName},
			    std::tuple{params.toJSON(),
			        params.target,
			        static_cast<Vizzu::Events::OnLineDrawEvent &>(
			            params)
			            .line});
		}
		else if (marker) {
			events.emplace(std::piecewise_construct,
			    std::tuple{params.eventName},
			    std::tuple{params.toJSON(),
			        params.target,
			        static_cast<Vizzu::Events::OnRectDrawEvent &>(
			            params)
			            .rect});
		}
		else
			events.emplace(std::piecewise_construct,
			    std::tuple{params.eventName},
			    std::tuple{params.toJSON(), params.target});
	};

	for (const auto &name : eventNames) {
		auto n = chart.getEventDispatcher().getEvent(name);
		skip->*n != nullptr;
		n->attach(0, event_handler);
	}

	using clock_t = std::chrono::steady_clock;
	chart.getAnimControl().update(clock_t::now());
	chart.setBoundRect(chart.getLayout().boundary);
	chart.draw(MyCanvas{}.getCanvas());
	chart.getAnimControl().update(clock_t::now());

	skip->*ends == "finished"_is_true;
	return events;
}

using enum Vizzu::Gen::ChannelId;
using enum Vizzu::Gen::ShapeType;
using enum Vizzu::Gen::CoordSystem;
using std::ranges::subrange;
using std::ranges::views::values;

const static auto tests =
    "Chart::Events"_suite

    | "area" |
    [](Vizzu::Chart &chart =
            chart_setup{
                {{x, "Dim5"}, {y, "Meas1"}, {label, "Meas1"}}})
{
	chart.getOptions().geometry = area;

	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 2u;
	check->*events.count("plot-axis-label-draw") == 4u + 5u;
	check->*events.count("plot-marker-label-draw") == 5u;
	check->*events.count("plot-marker-draw") == 4u;

	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[v, t, d] : values(subrange(beg, end)))
		check->*std::get<Vizzu::Draw::Rect>(d).rect.bottom() == 0.0;
}

    | "bar stacked rectangle negative" |
    [](Vizzu::Chart &chart =
            chart_setup{{{x, "Dim5"}, {x, "Meas2"}, {y, "Dim3"}}})
{
	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 2u;
	check->*events.count("plot-axis-label-draw") == 3u + 3u;
	check->*events.count("plot-marker-draw") == 10u;

	double xCenter{};
	using Axis = Vizzu::Events::Targets::Axis;
	for (auto &&[beg, end] = events.equal_range("plot-axis-draw");
	     const auto &[j, t, l] : values(subrange(beg, end)))
		if (!static_cast<Axis const &>(*t).horizontal)
			xCenter = std::get<Vizzu::Draw::Line>(l).line.begin.x;

	std::set<double> zero_count{};
	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[j, t, r] : values(subrange(beg, end)))
		if (auto rect = std::get<Vizzu::Draw::Rect>(r).rect;
		    std::abs(rect.pos.x - xCenter) <= 0.000001
		    || std::abs(rect.pos.x + rect.size.x - xCenter)
		           <= 0.000001)
			zero_count.insert(rect.bottom());
	check->*zero_count.size() == 3u;
}

    | "dotplot circle" |
    [](Vizzu::Chart &chart = chart_setup{{{x, "Dim5"}, {x, "Meas1"}}})
{
	chart.getOptions().geometry = circle;
	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 2u;
	check->*events.count("plot-marker-draw") == 5u;
	check->*events.count("plot-marker-guide-draw") == 5u;

	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[j, t, r] : values(subrange(beg, end))) {
		auto rect = std::get<Vizzu::Draw::Rect>(r).rect;
		check->*std::abs(rect.center().y - 0.5) <= 0.000001;
	}
}

    | "Marimekko chart" |
    [](Vizzu::Chart &chart =
            chart_setup{{{x, "Dim5"}, {x, "Meas1"}, {y, "Meas1"}}})
{
	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 2u;
	check->*events.count("plot-marker-draw") == 5u;

	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[j, t, r] : values(subrange(beg, end)))
		check->*std::get_if<Vizzu::Draw::Rect>(&r)->rect.bottom()
		    == 0.0;
}

    | "spiderweb area" |
    [](Vizzu::Chart &chart =
            chart_setup{{{x, "Dim5"}, {y, "Dim3"}, {y, "Meas1"}}})
{
	chart.getOptions().geometry = area;
	chart.getOptions().coordSystem = polar;

	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 2u;
	check->*events.count("plot-axis-label-draw") == 4u + 5u;
	check->*events.count("plot-marker-draw") == 13u;

	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[v, t, d] : values(subrange(beg, end)))
		check->*std::get_if<Vizzu::Draw::Rect>(&d) != nullptr;
}

    | "coxcomb" |
    [](Vizzu::Chart &chart =
            chart_setup{{{x, "Dim5"}, {y, "Meas1"}, {color, "Dim5"}}})
{
	chart.getOptions().coordSystem = polar;
	chart.getStyles().plot.marker.rectangleSpacing = 0;

	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 2u;
	check->*events.count("plot-axis-label-draw") == 4u + 5u;
	check->*events.count("plot-marker-draw") == 5u;

	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[v, t, d] : values(subrange(beg, end)))
		check->*std::get_if<Vizzu::Draw::Rect>(&d)->rect.bottom()
		    == 0.0;
}

    | "bubble chart" |
    [](Vizzu::Chart &chart =
            chart_setup{{{color, "Dim5"}, {size, "Meas1"}}})
{
	chart.getOptions().geometry = circle;

	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 0u;
	check->*events.count("plot-marker-draw") == 5u;

	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[v, t, d] : values(subrange(beg, end))) {
		auto rect = std::get<Vizzu::Draw::Rect>(d).rect;
		check->*std::abs(rect.size.x - rect.size.y) <= 0.000001;
	}
}

    | "stacked tree map" |
    [](Vizzu::Chart &chart = chart_setup{{{color, "Dim5"},
           {size, "Meas1"},
           {size, "Dim3"},
           {label, "Meas1"}}})
{
	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 0u;
	check->*events.count("plot-marker-draw") == 10u;

	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[v, t, d] : values(subrange(beg, end)))
		check->*std::get_if<Vizzu::Draw::Rect>(&d) != nullptr;
};