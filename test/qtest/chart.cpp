#include "chart.h"

#include <chart/generator/plot.h>

#include "base/io/log.h"
#include "chart/main/events.h"
#include "chart/rendering/drawplot.h"
#include "chart/rendering/logo.h"
#include "chart/ui/events.h"
#include "dataframe/old/datatable.h"

TestChart::TestChart() {}

void TestChart::prepareData()
{
	std::vector<const char *> cat1{"A", "B", "C"};
	std::vector<const char *> cat2{"aasd",
	    "bíyx",
	    "cAxyyxc",
	    "aS",
	    "bascxasc",
	    "cas asx",
	    "aasx asd aa",
	    "basd",
	    "c sad asdasd"};
	std::vector<double>
	    val{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
	auto &table = chart.getChart().getTable();
	table.addColumn("Cat1",
	    std::span(cat1),
	    std::array{0u, 0u, 0u, 1u, 1u, 1u, 2u, 2u, 2u});
	table.addColumn("Cat2",
	    std::span(cat2),
	    std::array{0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u});
	table.addColumn("Val", "", std::span(val));

	chart.getChart()
	    .getEventDispatcher()
	    .getEvent("pointermove")
	    ->attach(std::ref(*this));
}

void TestChart::operator()(Util::EventDispatcher::Params &params,
    const std::string &)
{
	std::optional<Vizzu::Gen::Marker::MarkerIndex> markerId;
	using Marker = Vizzu::Events::Targets::Marker;
	using MarkerChild = Vizzu::Events::Targets::MarkerChild;

	auto marker = dynamic_cast<const Marker *>(params.target);
	if (!marker)
		if (auto c = dynamic_cast<const MarkerChild *>(params.target))
			marker = &c->parent;
	if (marker) markerId.emplace(marker->marker.idx);

	chart.getChart().getOptions().showTooltip(markerId);
	chart.getChart().setKeyframe();
	chart.getChart().animate({});
}

void TestChart::run()
{
	using Vizzu::Gen::ChannelId;
	using Vizzu::Gen::PlotPtr;

	prepareData();

	auto end = [](const PlotPtr &, const bool &)
	{
		IO::log() << "finished";
	};

	auto step6 = [end, this](const PlotPtr &, const bool &)
	{
		IO::log() << "step 6";
		auto &options = chart.getChart().getOptions();
		options.title = "VIZZU Chart - Phase 6";
		options.showTooltip({});
		chart.getChart().setKeyframe();
		chart.getChart().animate({end});
	};

	auto step5 = [step6, this](const PlotPtr &, const bool &)
	{
		IO::log() << "step 5";
		auto &options = chart.getChart().getOptions();
		options.title = "VIZZU Chart - Phase 5";
		options.showTooltip("Cat1:A;Cat2:bíyx;");
		chart.getChart().setKeyframe();
		chart.getChart().animate({step6});
	};

	auto step4 = [step5, this](const PlotPtr &, const bool &)
	{
		IO::log() << "step 4";
		auto &options = chart.getChart().getOptions();
		options.title = "VIZZU Chart - Phase 4";
		options.showTooltip("Cat1:A;Cat2:aasd;");
		chart.getChart().setKeyframe();
		chart.getChart().animate({step5});
	};

	auto step3 = [step4, this](const PlotPtr &, const bool &)
	{
		IO::log() << "step 3";
		auto &options = chart.getChart().getOptions();
		auto &channels = options.getChannels();
		auto &table = chart.getChart().getTable();

		channels.at(ChannelId::y).removeSeries({"Cat2", table});
		channels.at(ChannelId::x).addSeries({"Cat2", table});
		options.title = "VIZZU Chart - Phase 3";
		chart.getChart().getStyles().title.textAlign =
		    ::Anim::Interpolated<Vizzu::Styles::Text::TextAlign>(
		        Vizzu::Styles::Text::TextAlign::right);
		chart.getChart().setKeyframe();
		chart.getChart().animate({step4});
	};

	auto step2 = [step3, this](const PlotPtr &, const bool &)
	{
		IO::log() << "step 2";
		auto &options = chart.getChart().getOptions();
		auto &styles = chart.getChart().getStyles();
		auto &table = chart.getChart().getTable();
		auto &channels = options.getChannels();

		options.dataFilter = Vizzu::Data::Filter{};

		channels.at(ChannelId::y).addSeries({"Cat2", table});
		channels.at(ChannelId::color).addSeries({"Cat2", table});
		options.coordSystem = Vizzu::Gen::CoordSystem::polar;
		options.title = "VIZZU Chart - Phase 2";
		styles.title.fontSize = Gfx::Length{10};
		styles.legend.marker.type =
		    Vizzu::Styles::Legend::Marker::Type::square;
		styles.title.textAlign = ::Anim::Interpolated(
		    Vizzu::Styles::Text::TextAlign::center);
		chart.getChart().setKeyframe();
		chart.getChart().animate({step3});
	};

	auto step1b = [step2, this](const PlotPtr &, const bool &)
	{
		try {
			IO::log() << "step 1b";
			auto &options = chart.getChart().getOptions();
			auto &styles = chart.getChart().getStyles();
			options.dataFilter = Vizzu::Data::Filter{
			    std::unique_ptr<bool(const Vizzu::Data::RowWrapper *),
			        void (*)(bool(const Vizzu::Data::RowWrapper *))>{
			        +[](const Vizzu::Data::RowWrapper *row) -> bool
			        {
				        return *std::get<const std::string *>(
				                   row->get_value("Cat1"))
				                == std::string_view{"A"}
				            || *std::get<const std::string *>(
				                   row->get_value("Cat2"))
				                   == std::string_view{"b"};
			        },
			        +[](bool(const Vizzu::Data::RowWrapper *))
			        {
			        }}};
			options.title = "VIZZU Chart - Phase 1b";
			styles.legend.marker.type =
			    Vizzu::Styles::Legend::Marker::Type::circle;
			styles.title.textAlign = ::Anim::Interpolated(
			    Vizzu::Styles::Text::TextAlign::right);
			chart.getChart().setKeyframe();
			chart.getChart().animate({step2});
		}
		catch (const std::exception &e) {
			IO::log() << e.what();
		}
	};

	auto step1 = [step1b, this]
	{
		IO::log() << "step 1";
		auto &options = chart.getChart().getOptions();
		auto &styles = chart.getChart().getStyles();
		auto &table = chart.getChart().getTable();
		auto &channels = options.getChannels();
		channels.at(ChannelId::x).addSeries({"Cat1", table});
		channels.at(ChannelId::x).addSeries({"exists()", table});
		channels.at(ChannelId::y).addSeries({"Val", table});
		channels.at(ChannelId::label).addSeries({"Val", table});
		channels.at(ChannelId::x).addSeries({"Val", table});
		channels.at(ChannelId::y).addSeries({"Cat2", table});
		channels.at(ChannelId::color).addSeries({"Cat2", table});
		styles.plot.marker.label.filter =
		    Gfx::ColorTransform::Lightness(0.5);
		styles.plot.marker.label.position =
		    Vizzu::Styles::MarkerLabel::Position::center;
		styles.legend.marker.type =
		    Vizzu::Styles::Legend::Marker::Type::square;
		styles.title.textAlign = ::Anim::Interpolated(
		    Vizzu::Styles::Text::TextAlign::left);
		options.title = "Example VIZZU Chart";
		chart.getChart().setKeyframe();
		chart.getChart().animate({step1b});
	};

	step1();
}
