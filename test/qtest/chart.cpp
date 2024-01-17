#include "chart.h"

#include "base/io/log.h"
#include "chart/main/events.h"
#include "chart/rendering/drawplot.h"
#include "chart/rendering/logo.h"
#include "chart/ui/events.h"
#include "data/datacube/datacube.h"

TestChart::TestChart() : chart() {}

void TestChart::prepareData()
{
	std::vector<std::string>
	    cat1{"A", "A", "A", "B", "B", "B", "C", "C", "C"};
	std::vector<std::string> cat2{"aasd",
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
	table.addColumn("Cat1", std::span(cat1));
	table.addColumn("Cat2", std::span(cat2));
	table.addColumn("Val", "", std::span(val));

	chart.getChart()
	    .getEventDispatcher()
	    .getEvent("pointermove")
	    ->attach(0, std::ref(*this));
}

void TestChart::operator()(Util::EventDispatcher::Params &params)
{
	std::optional<uint64_t> markerId;
	using Marker = Vizzu::Events::Targets::Marker;
	using MarkerChild = Vizzu::Events::Targets::MarkerChild;

	auto marker = dynamic_cast<const Marker *>(params.target);
	if (!marker)
		if (auto c = dynamic_cast<const MarkerChild *>(params.target))
			marker = &c->parent;
	if (marker) markerId.emplace(marker->marker.idx);

	chart.getChart().getSetter().showTooltip(markerId);
	chart.getChart().setKeyframe();
	chart.getChart().animate();
}

void TestChart::run()
{
	using Vizzu::Gen::ChannelId;

	prepareData();

	auto end = [=](bool)
	{
		IO::log() << "finished";
	};

	auto step6 = [end, this](bool)
	{
		IO::log() << "step 6";
		auto setter = chart.getChart().getSetter();
		setter.getOptions().title = "VIZZU Chart - Phase 6";
		setter.deleteMarkerInfo(5);
		chart.getChart().setKeyframe();
		chart.getChart().animate(end);
	};

	auto step5 = [step6, this](bool)
	{
		IO::log() << "step 5";
		auto setter = chart.getChart().getSetter();
		setter.getOptions().title = "VIZZU Chart - Phase 5";
		setter.moveMarkerInfo(4, 5);
		chart.getChart().setKeyframe();
		chart.getChart().animate(step6);
	};

	auto step4 = [step5, this](bool)
	{
		IO::log() << "step 4";
		auto setter = chart.getChart().getSetter();
		setter.getOptions().title = "VIZZU Chart - Phase 4";
		setter.addMarkerInfo(4);
		chart.getChart().setKeyframe();
		chart.getChart().animate(step5);
	};

	auto step3 = [step4, this](bool)
	{
		IO::log() << "step 3";
		auto setter = chart.getChart().getSetter();
		setter.deleteSeries(ChannelId::y, "Cat2");
		setter.addSeries(ChannelId::x, "Cat2");
		setter.getOptions().title = "VIZZU Chart - Phase 3";
		chart.getChart().getStyles().title.textAlign =
		    ::Anim::Interpolated<Vizzu::Styles::Text::TextAlign>(
		        Vizzu::Styles::Text::TextAlign::right);
		chart.getChart().setKeyframe();
		chart.getChart().animate(step4);
	};

	auto step2 = [step3, this](bool)
	{
		IO::log() << "step 2";
		auto setter = chart.getChart().getSetter();
		setter.setFilter(Vizzu::Data::Filter());
		setter.addSeries(ChannelId::y, "Cat2");
		setter.addSeries(ChannelId::color, "Cat2");
		setter.getOptions().coordSystem =
		    Vizzu::Gen::CoordSystem::polar;
		setter.getOptions().title = "VIZZU Chart - Phase 2";
		chart.getChart().getStyles().title.fontSize = Gfx::Length{10};
		chart.getChart().getStyles().legend.marker.type =
		    Vizzu::Styles::Legend::Marker::Type::square;
		chart.getChart().getStyles().title.textAlign =
		    ::Anim::Interpolated<Vizzu::Styles::Text::TextAlign>(
		        Vizzu::Styles::Text::TextAlign::center);
		chart.getChart().setKeyframe();
		chart.getChart().animate(step3);
	};

	auto step1b = [step2, this](bool)
	{
		try {
			IO::log() << "step 1b";
			auto setter = chart.getChart().getSetter();
			setter.setFilter(Vizzu::Data::Filter(
			    [&](const auto &row)
			    {
				    return *row["Cat1"] == row["Cat1"]["A"]
				        || static_cast<std::string>(row["Cat2"])
				               == "b";
			    },
			    0));
			setter.getOptions().title = "VIZZU Chart - Phase 1b";
			chart.getChart().getStyles().legend.marker.type =
			    Vizzu::Styles::Legend::Marker::Type::circle;
			chart.getChart().getStyles().title.textAlign =
			    ::Anim::Interpolated<Vizzu::Styles::Text::TextAlign>(
			        Vizzu::Styles::Text::TextAlign::right);
			chart.getChart().setKeyframe();
			chart.getChart().animate(step2);
		}
		catch (const std::exception &e) {
			IO::log() << e.what();
		}
	};

	auto step1 = [step1b, this](bool)
	{
		IO::log() << "step 1";
		auto setter = chart.getChart().getSetter();
		setter.addSeries(ChannelId::x, "Cat1");
		setter.addSeries(ChannelId::x, "exists()");
		setter.addSeries(ChannelId::y, "Val");
		setter.addSeries(ChannelId::label, "Val");
		setter.addSeries(ChannelId::x, "Val");
		setter.addSeries(ChannelId::y, "Cat2");
		setter.addSeries(ChannelId::color, "Cat2");
		chart.getChart().getStyles().plot.marker.label.filter =
		    Gfx::ColorTransform::Lightness(0.5);
		chart.getChart().getStyles().plot.marker.label.position =
		    Vizzu::Styles::MarkerLabel::Position::center;
		chart.getChart().getStyles().legend.marker.type =
		    Vizzu::Styles::Legend::Marker::Type::square;
		chart.getChart().getStyles().title.textAlign =
		    ::Anim::Interpolated<Vizzu::Styles::Text::TextAlign>(
		        Vizzu::Styles::Text::TextAlign::left);
		setter.getOptions().title = "Example VIZZU Chart";
		chart.getChart().setKeyframe();
		chart.getChart().animate(step1b);
	};

	step1(true);
}
