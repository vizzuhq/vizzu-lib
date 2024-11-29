
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
	void setBrushGradient(const Gfx::LinearGradient &) final {}
	void frameBegin() final {}
	void frameEnd() final {}
	void *getPainter() final { return static_cast<Painter *>(this); }
	// cppcheck-suppress duplInheritedMember
	ICanvas &getCanvas() final { return *this; }
};

auto testcase_0 = [](Vizzu::Data::DataTable &table)
{
	table.add_dimension(std::initializer_list<const char *>{},
	    std::initializer_list<std::uint32_t>{},
	    "Index");
	table.add_measure(std::initializer_list<double>{}, "x");
	table.add_measure(std::initializer_list<double>{}, "y");
};

auto testcase_1 = [](Vizzu::Data::DataTable &table)
{
	table.add_dimension({{"A", "B", "C", "D", "E"}},
	    {{0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4}},
	    "Dim5");
	table.add_dimension({{"a", "b"}},
	    {{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}},
	    "Dim2");
	table.add_dimension({{"a", "b", "c"}},
	    {{0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 1, 0, 2, 2, 1, 0}},
	    "Dim3");
	table.add_measure(
	    {{1, 2, 4, 3, 3, 4, 2, 1, 4, 3, 1, 2, 2, 1, 3, 4}},
	    "Meas1");
	table.add_measure(
	    {{0, -1, 5, 6, 6, 5, -1, 0, 5, 6, 0, -1, -1, 0, 6, -5}},
	    "Meas2");
};

auto testcase_2 = [](Vizzu::Data::DataTable &table)
{
	table.add_dimension({{
	                        "Long name wich has no end",
	                        R"(Raw
break)",
	                        R"(キャラクターセット)",
	                    }},
	    {{0,
	        0,
	        0,
	        0,
	        1,
	        1,
	        1,
	        1,
	        2,
	        2,
	        2,
	        2,
	        0,
	        0,
	        0,
	        0,
	        1,
	        1,
	        1,
	        1,
	        2,
	        2,
	        2,
	        2,
	        0,
	        0,
	        0,
	        0,
	        1,
	        1,
	        1,
	        1,
	        2,
	        2,
	        2,
	        2,
	        0,
	        0,
	        0,
	        0,
	        1,
	        1,
	        1,
	        1,
	        2,
	        2,
	        2,
	        2}},
	    "Channel title for long names");

	table.add_dimension({{"Very long label of this element",
	                        "",
	                        "It is also long enough",
	                        "Short one",
	                        "Jap",
	                        "キャラクターセット",
	                        R"(Raw
break)",
	                        "h",
	                        "i",
	                        "j"}},
	    {{
	        0,
	        1,
	        2,
	        3,
	        4,
	        5,
	        6,
	        7,
	        8,
	        9,
	        0,
	        1,
	        2,
	        3,
	        4,
	        5,
	        6,
	        7,
	        8,
	        9,
	        0,
	        1,
	        2,
	        3,
	        4,
	        5,
	        6,
	        7,
	        8,
	        9,
	        0,
	        1,
	        2,
	        3,
	        4,
	        5,
	        6,
	        7,
	        8,
	        9,

	    }},
	    "Childs of long names which have no end");

	table.add_measure({{639,
	                      354,
	                      278,
	                      312,
	                      1241,
	                      1512,
	                      863,
	                      789,
	                      765,
	                      653,
	                      542,
	                      497,
	                      673,
	                      412,
	                      308,
	                      345,
	                      1329,
	                      1671,
	                      962,
	                      821,
	                      798,
	                      681,
	                      584,
	                      518,
	                      706,
	                      432,
	                      326,
	                      358,
	                      1382,
	                      1715,
	                      1073,
	                      912,
	                      821,
	                      721,
	                      618,
	                      542,
	                      721,
	                      462,
	                      372,
	                      367,
	                      1404,
	                      1729,
	                      1142,
	                      941,
	                      834,
	                      778,
	                      651,
	                      598}},
	    "値3");
};

struct chart_setup
{
	struct series_t
	{
		Vizzu::Gen::ChannelId channel;
		const char *name;
		const char *aggr{};
	};
	std::vector<series_t> series;
	std::function<void(Vizzu::Data::DataTable &)> testcase =
	    testcase_1;
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
		testcase(table);
		auto &channels = chart.getOptions().getChannels();
		for (auto &&[ch, name, aggr] : series) {
			channels.at(ch).addSeries({name, std::ref(table)});
			if (aggr) {
				if (!channels.at(ch).measure())
					channels.at(ch).set.measureId =
					    channels.at(ch).set.dimensionIds.pop_back();
				channels.at(ch).set.measureId->setAggr(aggr);
			}
		}
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

	static bool ends{};
	chart.setKeyframe();
	ends = false;
	chart.animate({[](const Vizzu::Gen::PlotPtr &, const bool &b)
	    {
		    ends = b;
	    }});

	std::multimap<std::string, event_as, std::less<>> events;

	static std::vector<std::string> eventNames = []
	{
		std::vector<std::string> names;
		Refl::visit<Vizzu::Events::DrawEvents>(
		    [&names]<class T>(
		        Util::EventDispatcher::event_ptr &(*)(const T &),
		        const std::initializer_list<std::string_view> &sv)
		    {
			    std::span s{sv};
			    if (s.back() == "begin" || s.back() == "complete") {
				    return names.emplace_back(
				        "draw-" + std::string{s.back()});
			    }

			    // cppcheck-suppress uselessCallsConstructor
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
	auto event_handler =
	    [&events, &line](Util::EventDispatcher::Params &params,
	        const std::string &json)
	{
		auto marker = params.eventName == "plot-marker-draw";
		if ((marker && line)
		    || params.eventName == "plot-axis-draw") {
			events.emplace(std::piecewise_construct,
			    std::tuple{params.eventName},
			    std::tuple{json,
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
		n->attach(event_handler);
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

	check->*events.count("plot-axis-draw") == 1u;
	check->*events.count("plot-axis-label-draw") == 4u + 5u;
	check->*events.count("plot-marker-label-draw") == 5u;
	check->*events.count("plot-marker-draw") == 5u;

	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[v, t, d] : values(subrange(beg, end)))
		check->*std::get<Vizzu::Draw::Rect>(d).rect.bottom() == 0.0;
}

    | "bar stacked rectangle negative" |
    [](Vizzu::Chart &chart =
            chart_setup{{{x, "Dim5"}, {x, "Meas2"}, {y, "Dim3"}}})
{
	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 1u;
	check->*events.count("plot-axis-label-draw") == 3u + 3u;
	check->*events.count("plot-marker-draw") == 10u;

	double xCenter{};
	using Axis = Vizzu::Events::Targets::Axis;
	for (auto &&[beg, end] = events.equal_range("plot-axis-draw");
	     const auto &[j, t, l] : values(subrange(beg, end)))
		if (!isHorizontal(
		        orientation(static_cast<Axis const &>(*t).axis)))
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

	check->*events.count("plot-axis-draw") == 1u;
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

	check->*events.count("plot-axis-draw") == 1u;
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

	check->*events.count("plot-axis-draw") == 1u;
	check->*events.count("plot-axis-label-draw") == 4u + 5u;
	check->*events.count("plot-marker-draw") == 10u;

	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[v, t, d] : values(subrange(beg, end)))
		check->*std::get_if<Vizzu::Draw::Rect>(&d) != nullptr;
}

    | "coxcomb" |
    [](Vizzu::Chart &chart =
            chart_setup{{{x, "Dim5"}, {y, "Meas1"}, {color, "Dim5"}}})
{
	chart.getOptions().coordSystem = polar;

	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 1u;
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
}

    | "icicle rectangle 2dis 1con" |
    [](Vizzu::Chart &chart = chart_setup{{{x, "Dim5"},
           {x, "Meas1"},
           {y, "Dim2"},
           {y, "Meas2"}}})
{
	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 1u;
	check->*events.count("plot-marker-draw") == 5u;

	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[v, t, d] : values(subrange(beg, end)))
		check->*std::get_if<Vizzu::Draw::Rect>(&d) != nullptr;
}

    | "53978116" |
    [](Vizzu::Chart &chart = chart_setup{{{color, "Dim3"},
           {x, "Dim5"},
           {y, "Meas1", "min"},
           {y, "Dim3"}}})
{
	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 1u;
	check->*events.count("plot-marker-draw") == 10u;

	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[v, t, d] : values(subrange(beg, end)))
		check->*std::get_if<Vizzu::Draw::Rect>(&d) != nullptr;
}

    | "aggregators step1" |
    [](Vizzu::Chart &chart = chart_setup{{{x, "Dim3"},
           {y, "Meas1", "min"},
           {label, "Meas1", "min"}}})
{
	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 1u;
	check->*events.count("plot-marker-draw") == 3u;

	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[v, t, d] : values(subrange(beg, end)))
		check->*std::get_if<Vizzu::Draw::Rect>(&d) != nullptr;
}

    | "column rectangle less disc" |
    [](Vizzu::Chart &chart =
            chart_setup{
                {{y, "Channel title for long names"},
                    {y, "値3"},
                    {x, "Childs of long names which have no end"},
                    {color, "Channel title for long names"},
                    {label, "Channel title for long names"}},
                testcase_2})
{
	chart.getOptions().getChannels().at(y).range.min =
	    Vizzu::Base::AutoParam{
	        Vizzu::Gen::ChannelExtrema::fromString("110%")};
	chart.getOptions().getChannels().at(y).range.max =
	    Vizzu::Base::AutoParam{
	        Vizzu::Gen::ChannelExtrema::fromString("0%")};
	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 1u;
	check->*events.count("plot-marker-draw") == 28u;

	for (auto &&[beg, end] = events.equal_range("plot-marker-draw");
	     const auto &[v, t, d] : values(subrange(beg, end)))
		check->*std::get_if<Vizzu::Draw::Rect>(&d) != nullptr;
}

    | "paint" |
    [](Vizzu::Chart &chart =
            chart_setup{{{noop, "Index"}, {x, "x"}, {y, "y"}},
                testcase_0})
{
	chart.getOptions().getChannels().at(y).range.min =
	    Vizzu::Base::AutoParam{
	        Vizzu::Gen::ChannelExtrema::fromString("0.0")};
	chart.getOptions().getChannels().at(y).range.max =
	    Vizzu::Base::AutoParam{
	        Vizzu::Gen::ChannelExtrema::fromString("1.0")};
	chart.getOptions().getChannels().at(x).range.min =
	    Vizzu::Base::AutoParam{
	        Vizzu::Gen::ChannelExtrema::fromString("0.0")};
	chart.getOptions().getChannels().at(x).range.max =
	    Vizzu::Base::AutoParam{
	        Vizzu::Gen::ChannelExtrema::fromString("1.0")};

	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 2u;
	check->*events.count("plot-axis-label-draw") == 4u + 4u;
	check->*events.count("plot-axis-title-draw") == 2u;
	check->*events.count("plot-marker-draw") == 0u;
}

    | "empty_chart" |
    [](Vizzu::Chart &chart =
            chart_setup{{{y, "Index"}, {x, "x"}}, testcase_0})
{
	auto &&events = get_events(chart);

	check->*events.count("plot-axis-draw") == 1u;
	check->*events.count("plot-axis-label-draw") == 1u;
	check->*events.count("plot-axis-title-draw") == 2u;
	check->*events.count("plot-marker-draw") == 0u;
};