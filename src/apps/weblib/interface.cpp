#include "interface.h"

#include <sstream>
#include <span>

#include "base/io/log.h"
#include "base/io/memorystream.h"
#include "base/text/jsonoutput.h"
#include "jscriptcanvas.h"

#include "interfacejs.h"

#include "functionwrapper.h"

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
	return objects.reg(snapshot);
}

void Interface::restoreChart(void *chartPtr)
{
	auto snapshot = objects.get<Snapshot>(chartPtr);
	chart->getChart().setOptions(snapshot->options);
	chart->getChart().setStyles(snapshot->styles);
}

void *Interface::storeAnim()
{
	auto animation = chart->getChart().getAnimation();
	auto anim = std::make_shared<Animation>(
		animation,
		Snapshot(
			chart->getChart().getOptions(), 
			chart->getChart().getStyles())
		);

	return objects.reg(anim);
}

void Interface::restoreAnim(void *animPtr)
{
	auto anim = objects.get<Animation>(animPtr);
	chart->getChart().setAnimation(anim->animation);
	// todo: followings should be passed in setAnimation too
	chart->getChart().setOptions(anim->snapshot.options);
	chart->getChart().setStyles(anim->snapshot.styles);
}

void Interface::freeObj(void *ptr)
{
	objects.unreg(ptr);
}

const char *Interface::getStyleList()
{
	static std::string res = Text::toJSon(Styles::Sheet::paramList());
	return res.c_str();
}

const char *Interface::getStyleValue(const char *path, bool computed)
{
	if (chart)
	{
		static std::string res;
		auto &styles = computed
			? chart->getChart().getComputedStyles()
			: chart->getChart().getStyles();
		res = Styles::Sheet::getParam(styles, path);
		return res.c_str();
	}
	else throw std::logic_error("No chart exists");
}

void Interface::setStyleValue(const char *path, const char *value)
{
	if (chart)
	{
		chart->getChart().getStylesheet().setParams(path, value);
	}
	else throw std::logic_error("No chart exists");
}

const char *Interface::getChartParamList()
{
	static std::string res = Text::toJSon(Diag::Config::listParams());
	return res.c_str();
}

const char *Interface::getChartValue(const char *path)
{
	if (chart)
	{
		static std::string res;
		res = chart->getChart().getConfig().getParam(path); 
		return res.c_str();
	}
	else throw std::logic_error("No chart exists");
}

void Interface::setChartValue(const char *path, const char *value)
{
	try
	{
		if (chart)
		{
			chart->getChart().getConfig().setParam(path, value);
		}
		else throw std::logic_error("No chart exists");
	}
	catch (std::exception &e)
	{
		throw std::logic_error(
			std::string(path) + "/" + value + ": " + e.what()
		);
	}
}

void Interface::relToCanvasCoords(double rx, double ry, double &x, double &y)
{
	if (chart)
	{
		Geom::Point from(rx, ry);
		auto to = chart->getChart().getCoordSystem().convert(from);
		x = to.x;
		y = to.y;
	}
	else throw std::logic_error("No chart exists");
}

void Interface::canvasToRelCoords(double x, double y, double &rx, double &ry)
{
	if (chart)
	{
		Geom::Point from(x, y);
		auto to = chart->getChart().getCoordSystem().getOriginal(from);
		rx = to.x;
		ry = to.y;
	}
	else throw std::logic_error("No chart exists");
}

void Interface::setChartFilter(bool (*filter)(const void *))
{
	if (chart)
	{
		auto wrappedFilter = FunctionWrapper<bool, Data::RowWrapper>::wrap(filter);
		chart->getChart().getConfig().setFilter(wrappedFilter, (intptr_t)filter);
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

void Interface::animate(void (*callback)(bool))
{
	if (chart) chart->getChart().animate([=](bool ok){ callback(ok); });
	else throw std::logic_error("No chart exists");
}

void Interface::setKeyframe()
{
	if (chart) chart->getChart().setKeyframe();
	else throw std::logic_error("No chart exists");
}

const char *Interface::getMarkerData(unsigned id)
{
	if (chart 
		&& chart->getChart().getDiagram()) 
	{
		static std::string res;
		const auto *marker = chart->getChart().markerByIndex(id);
		if (marker) res = marker->toJson(chart->getChart().getDiagram()->getTable());
		return res.c_str();
	}
	else throw std::logic_error("No chart exists");
}

void Interface::animControl(const char *command, const char *param)
{
	if (chart) {
		auto &ctrl = chart->getChart().getAnimControl();
		std::string cmd(command);
		if (cmd == "seek") ctrl.seek(param);
		else if (cmd == "pause") ctrl.pause();
		else if (cmd == "play") ctrl.play();
		else if (cmd == "stop") ctrl.stop();
		else if (cmd == "cancel") ctrl.cancel();
		else if (cmd == "reverse") ctrl.reverse();
		else throw std::logic_error("invalid animation command");
	}
	else throw std::logic_error("No chart exists");
}

void Interface::setAnimValue(const char *path, const char *value)
{
	if (chart) {
		chart->getChart().getAnimOptions().set(path, value);
	}
}

void Interface::addDimension(const char *name,
    const char **categories,
    int count)
{
	if (chart && categories) {
		std::span<const char *> view(categories, count);
		auto &table = chart->getChart().getTable();
		table.addColumn(name, view);
	}
}

void Interface::addMeasure(const char *name,
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

void Interface::addRecord(const char **cells, int count)
{
	if (chart)
	{
		std::span<const char *> view(cells, count);
		auto &table = chart->getChart().getTable();
		table.pushRow(view);
	}
}

const char *Interface::dataMetaInfo()
{
	if (chart)
	{
		static std::string res;
		res.clear();
		auto &table = chart->getChart().getTable();
		res += "[";
		for (auto i = 0u; i < table.columnCount(); ++i)
		{
			res += table.getInfo(Data::ColumnIndex(i)).toJSon();
			if (i < table.columnCount() - 1) res += ",";
		}
		res += "]";
		return res.c_str();
	}
	else throw std::logic_error("No chart exists");
}

void Interface::init()
{
	IO::Log::set([=](const std::string &msg) {
		if (logging) log((msg + "\n").c_str());
	});

	taskQueue = std::make_shared<GUI::TaskQueue>();
	chart = std::make_shared<UI::ChartWidget>(taskQueue);
	chart->doChange = [&]{ needsUpdate = true; };
	chart->doSetCursor = [&](GUI::Cursor cursor) {
		::setCursor(GUI::toCSS(cursor));
	};
	chart->openUrl = [&](const std::string& url) {
		::openUrl(url.c_str());
	};
	needsUpdate = true;
}

void Interface::poll()
{
	if (taskQueue) taskQueue->poll();
}

void Interface::update(double width, double height, RenderControl renderControl)
{
	if (!chart) throw std::logic_error("No chart exists");

	auto now = std::chrono::steady_clock::now();
	chart->getChart().getAnimControl().update(now);
	
	Geom::Size size(width, height);
	
	bool renderNeeded = needsUpdate || chart->getBoundary().size != size;

	if ( (renderControl == allow && renderNeeded) || renderControl == force)
	{
		Vizzu::Main::JScriptCanvas canvas;
		canvas.frameBegin();
		chart->updateSize(canvas, size);
		chart->draw(canvas);
		canvas.frameEnd();
		needsUpdate = false;
	}
}

void Interface::pointerDown(int pointerId, double x, double y)
{
	if (chart)
	{
		chart->onPointerDown(GUI::PointerEvent(pointerId, Geom::Point(x, y)));
		needsUpdate = true;
	}
	else throw std::logic_error("No chart exists");
}

void Interface::pointerUp(int pointerId, double x, double y)
{
	if (chart)
	{
		chart->onPointerUp(GUI::PointerEvent(pointerId, Geom::Point(x, y)),
			GUI::DragObjectPtr());
		needsUpdate = true;
	}
	else throw std::logic_error("No chart exists");
}

void Interface::pointerLeave(int pointerId)
{
	if (chart)
	{
		chart->onPointerLeave(pointerId);
		needsUpdate = true;
	}
	else throw std::logic_error("No chart exists");
}

void Interface::wheel(double delta)
{
	if (chart)
	{
		chart->onWheel(delta);
		needsUpdate = true;
	}
	else throw std::logic_error("No chart exists");
}

void Interface::pointerMove(int pointerId, double x, double y)
{
	if (chart)
	{
		GUI::DragObjectPtr nodrag;
		chart->onPointerMove(GUI::PointerEvent(pointerId, Geom::Point(x, y)), 
			nodrag);
		needsUpdate = true;
	}
	else throw std::logic_error("No chart exists");
}

void Interface::keyPress(int key, bool ctrl, bool alt, bool shift)
{
	if (chart)
	{
		GUI::KeyModifiers keyModifiers(shift, ctrl, alt);
		chart->onKeyPress(GUI::Key(key), keyModifiers);
		needsUpdate = true;
	}
	else throw std::logic_error("No chart exists");
}

void Interface::log(const char *str) {
	jsconsolelog(str);
}
