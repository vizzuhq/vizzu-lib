#include "interface.h"

#include <sstream>
#include <span>

#include "base/io/log.h"
#include "base/io/memorystream.h"
#include "jscriptcanvas.h"

extern "C" {
	extern char* jsconsolelog(const char*);
	extern void setMouseCursor(const char *cursor);
	extern void event_invoked(int, const char*);
	extern void removeJsFunction(void *);
}

using namespace Util;
using namespace Vizzu;

Interface Interface::instance;

Interface::Interface() : versionStr(std::string(Main::version))
{
	needsUpdate = false;
	logging = false;
	eventParam = nullptr;
}

const char *Interface::version() const
{
	return versionStr.c_str();
}

void *Interface::storeChart()
{
	auto snapshot = std::make_shared<Snapshot>(
		chart->getChart().getOptions(), 
		chart->getChart().getStyles()
	);
	snapshots.emplace(snapshot.get(), snapshot);
	return snapshot.get();
}

void Interface::restoreChart(void *chartPtr)
{
	auto it = snapshots.find(chartPtr);
	if (it == snapshots.end() || !it->second) 
		throw std::logic_error("No such chart exists");
	chart->getChart().setOptions(it->second->options);
	chart->getChart().setStyles(it->second->styles);
}

void Interface::freeChart(void *chart)
{
	auto it = snapshots.find(chart);
	if (it == snapshots.end()) throw std::logic_error("No such chart exists");
	snapshots.erase(it);
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

void Interface::setChartFilter(bool (*filter)(const void *))
{
	if (chart)
	{
		chart->getChart().getDescriptor().setFilter(filter, 
			reinterpret_cast<void (*)(bool (*)(const void*))>
				(removeJsFunction));
	}
}

const void *Interface::getRecordValue(void *record,
    const char *column,
    bool discrete)
{
	auto &row = *static_cast<const Data::RowWrapper*>(record);
	auto cell = row[column];
	if (discrete)
		return static_cast<const void *>(cell.discreteValue());
	else
		return static_cast<const void *>(&(*cell));
}

int Interface::addEventListener(const char * event) {
	auto& ed = chart->getChart().getEventDispatcher();
	auto id = ed[event]->attach([&](EventDispatcher::Params& params) {
		eventParam = &params;
		auto jsonStrIn = params.toJsonString();
		event_invoked(params.handler, jsonStrIn.c_str());
		eventParam = nullptr;
	});
	return (int)id;
}

void Interface::removeEventListener(const char * event, int id) {
	auto& ed = chart->getChart().getEventDispatcher();
	ed[event]->detach(id);
}

void Interface::preventDefaultEvent()
{
	if (eventParam) eventParam->preventDefault = true;
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

void Interface::setAnimValue(const char *path, const char *value)
{
	if (chart) {
		chart->getChart().getAnimOptions().set(path, value);
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

void Interface::init()
{
	IO::Log::set([=](const std::string&msg) {
		if (logging) log((msg + "\n").c_str());
	});

	chart = std::make_shared<UI::ChartWidget>();
	chart->doChange = [&]{ needsUpdate = true; };
	chart->setMouseCursor = [&](GUI::Cursor cursor) {
		::setMouseCursor(GUI::toCSS(cursor));
	};
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
