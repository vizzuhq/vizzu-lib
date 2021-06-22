#include "chart.h"

#include "base/io/log.h"
#include "chart/rendering/drawdiagram.h"
#include "chart/rendering/logo.h"
#include "data/datacube/datacube.h"
#include "chart/main/events.h"

using namespace Vizzu;

TestChart::TestChart() : chart(GUI::ScreenInfo()) {}

void TestChart::prepareData()
{
	std::vector<std::string> cat1{ "A", "A", "A", "B", "B", "B", "C", "C", "C" };
	std::vector<std::string> cat2{ "a", "b", "c", "a", "b", "c", "a", "b", "c" };
	std::vector<double> val{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
	auto &table = chart.getChart().getTable();
	table.addColumn("Cat1", std::span(cat1));
	table.addColumn("Cat2", std::span(cat2));
	table.addColumn("Val", std::span(val));
}

void TestChart::run()
{
	using namespace Vizzu::Diag;

	prepareData();

	auto end = [=]
	{
		IO::log() << "finished";
	};

	auto step3 = [=]
	{
		IO::log() << "step 3";
		auto setter = chart.getChart().getSetter();
		setter->deleteSeries(Scale::Y, "Cat2");
		setter->addSeries(Scale::X, "Cat2");
		setter->setTitle("VIZZU Chart - Phase 3");
		chart.getChart().getStyles().title.textAlign =
		    ::Anim::Interpolated<Styles::Text::TextAlign>(
		        Styles::Text::TextAlign::right);
		chart.getChart().animate(end);
	};

	auto step2 = [=]
	{
		IO::log() << "step 2";
		auto setter = chart.getChart().getSetter();
		setter->addSeries(Scale::Y, "Cat2");
		setter->addSeries(Scale::Color, "Cat2");
		setter->setPolar(true);
		setter->setTitle("VIZZU Chart - Phase 2");
		chart.getChart().getStyles().title.fontSize = 10;

		chart.getChart().getStyles().plot.marker.label.position =
		    Styles::MarkerLabel::Position::above;
		chart.getChart().getStyles().legend.marker.type =
		    Styles::Legend::Marker::Type::square;
		chart.getChart().getStyles().title.textAlign =
		    ::Anim::Interpolated<Styles::Text::TextAlign>(
		        Styles::Text::TextAlign::center);
		chart.getChart().animate(step3);
	};

	auto step1b = [=]
	{
		try {
			IO::log() << "step 1b";
			auto descriptor = chart.getChart().getDescriptor();
			auto setter = chart.getChart().getSetter();
			descriptor.setParam("filter.0.Cat1", "A");
			descriptor.setParam("filter.0.Cat2", "a");
			descriptor.setParam("filter.1.Cat1", "C");
			descriptor.setParam("filter.1.Cat2", "c");
			descriptor.setParam("filter.2", "push");
			setter->setTitle("VIZZU Chart - Phase 1b");
			chart.getChart().getStyles().plot.marker.label.position =
			    Styles::MarkerLabel::Position::below;
			chart.getChart().getStyles().legend.marker.type =
			    Styles::Legend::Marker::Type::circle;
			chart.getChart().getStyles().title.textAlign =
			    ::Anim::Interpolated<Styles::Text::TextAlign>(
			        Styles::Text::TextAlign::right);
			chart.getChart().animate(step2);
		}
		catch (const std::exception &e)
		{
			IO::log() << e.what();
		}
	};

	auto step1 = [=]
	{
		IO::log() << "step 1";
		auto setter = chart.getChart().getSetter();
		setter->addSeries(Scale::X, "Cat1");
		setter->addSeries(Scale::X, "$exists");
		setter->addSeries(Scale::Y, "Val");
		setter->addSeries(Scale::Label, "Val");
		setter->addSeries(Scale::X, "Val");
		setter->addSeries(Scale::Y, "Cat2");
		setter->addSeries(Scale::Color, "Cat2");
		setter->setLegend(Scale::Color);
		chart.getChart().getStyles().plot.marker.label.filter =
		    Gfx::ColorTransform::Lightness(0.5);
		chart.getChart().getStyles().plot.marker.label.position =
		    Styles::MarkerLabel::Position::center;
		chart.getChart().getStyles().legend.marker.type =
		    Styles::Legend::Marker::Type::square;
		chart.getChart().getStyles().title.textAlign =
		    ::Anim::Interpolated<Styles::Text::TextAlign>(
		        Styles::Text::TextAlign::left);
		setter->setTitle("Example VIZZU Chart");
		chart.getChart().animate(step1b);
	};

	step1();
}
