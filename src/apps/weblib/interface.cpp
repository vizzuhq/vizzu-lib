#include "interface.h"

#include <sstream>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#endif

#include "base/io/log.h"
#include "base/io/memorystream.h"
#include "jscriptcanvas.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
#endif

extern "C" {
	extern char* jsconsolelog(const char*);
    extern char* event_invoked(int, const char*);
}

using namespace Util;
using namespace Vizzu;

Interface Interface::instance;

Interface::Interface() : versionStr(std::string(Main::version))
{
	needsUpdate = false;
	logging = false;
}

const char *Interface::version() const
{
	return versionStr.c_str();
}

void Interface::setStyleValue(const char *path, const char *value)
{
	try {
		if (chart)
		{
			if (chart->getChart().getStylesheet().hasParam(path))
				chart->getChart().getStylesheet().setParam(path, value);
			else
				throw std::logic_error(
				    "non-existent style parameter: " + std::string(path));
		}
	}
	catch(std::exception &e) {
		IO::log() << path << value << "error:" << e.what() << '\n';
	}
}

void Interface::setChartValue(const char *path, const char *value)
{
	try
	{
		if (chart)
		{
			chart->getChart().getDescriptor().setParam(path, value);
		}
	}
	catch (std::exception &e)
	{
		IO::log() << path << value << "error:" << e.what() << '\n';
	}
}

int Interface::addEventListener(const char * event) {
	auto& ed = chart->getChart().getEventDispatcher();
	auto id = ed[event]->attach([&](EventDispatcher::Params& params) {
		auto jsonStrIn = params.toJsonString();
		auto jsonStrOut = event_invoked(params.handler, jsonStrIn.c_str());
		if (jsonStrOut) {
			params.fromJsonString(jsonStrOut);
			free(jsonStrOut);
		}
	});
	return (int)id;
}

void Interface::removeEventListener(const char * event, int id) {
	auto& ed = chart->getChart().getEventDispatcher();
	ed[event]->detach(id);
}

void Interface::animate(void (*callback)())
{
	try
	{
		if (chart) chart->getChart().animate([=]{ callback(); });
	}
	catch (const std::exception &e)
	{
		IO::log() << e.what() << '\n';
	}
}

void Interface::animControl(const char *command, const char *param)
{
	try
	{
		if (chart) {
			auto &ctrl = chart->getChart().getAnimControl();
			std::string cmd(command);
			if (cmd == "seek") ctrl.seek(param);
			else if (cmd == "pause") ctrl.pause();
			else if (cmd == "play") ctrl.play();
			else if (cmd == "stop") ctrl.stop();
			else if (cmd == "reverse") ctrl.reverse();
			else throw std::logic_error("invalid animation command");
		}
	}
	catch (const std::exception &e)
	{
		IO::log() << e.what() << '\n';
	}
}

void Interface::addCategories(const char *name,
    const char **categories,
    int count)
{
	if (chart && categories) {
		std::span<const char *> view(categories, count);
		auto &table = chart->getChart().getTable();
		table.addColumn(name, view);
	}
}

void Interface::addValues(const char *name,
    double *values,
    int count)
{
	if (chart)
	{
		std::span<double> view(values, count);
		auto &table = chart->getChart().getTable();
		table.addColumn(name, view);
	}
}

void Interface::init(double dpi, double width_mm, double height_mm)
{
	IO::Log::set([=](const std::string&msg) {
		if (logging) log((msg + "\n").c_str());
	});

	GUI::ScreenInfo screenInfo{dpi, Geom::Size(width_mm, height_mm)};

	chart = std::make_shared<UI::ChartWidget>(screenInfo);
	chart->doChange = [&]{ needsUpdate = true; };
}

void Interface::poll()
{
	if (taskQueue) taskQueue->poll();
}

void Interface::update(double, double width, double height, bool force)
{
	if (!chart) {
		IO::log() << "no chart exists";
		return;
	}

	auto now = std::chrono::steady_clock::now();
	chart->getChart().getAnimControl().update(now);

	if (needsUpdate || force) {
		try {
			Geom::Size size(width, height);
			Vizzu::Main::JScriptCanvas canvas;
			canvas.frameBegin();
			chart->updateSize(canvas, size);
			chart->draw(canvas);
			canvas.frameEnd();
		}
		catch (std::exception &e) {
			IO::log() << "error" << e.what();
		}
		needsUpdate = false;
	}
}

void Interface::mouseDown(double x, double y)
{
	if (chart)
	{
		chart->onMouseDown(Geom::Point(x, y));
		needsUpdate = true;
	}
	else IO::log() << "no chart exists";
}

void Interface::mouseUp(double x, double y)
{
	if (chart)
	{
		chart->onMouseUp(Geom::Point(x, y), GUI::DragObjectPtr());
		needsUpdate = true;
	}
	else IO::log() << "no chart exists";
}

void Interface::mouseMove(double x, double y)
{
	if (chart)
	{
		GUI::DragObjectPtr nodrag;
		chart->onMouseMove(Geom::Point(x, y), nodrag);
		needsUpdate = true;
	}
	else IO::log() << "no chart exists";
}

void Interface::keyPress(int key, bool ctrl, bool alt, bool shift)
{
	if (chart)
	{
//		IO::log() << "key down" << key << ctrl << alt << shift;
		GUI::KeyModifiers keyModifiers(shift, ctrl, alt);
		chart->onKeyPress(GUI::Key(key), keyModifiers);
		needsUpdate = true;
	}
	else IO::log() << "no chart exists";
}

void Interface::log(const char *str) {
	jsconsolelog(str);
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif
