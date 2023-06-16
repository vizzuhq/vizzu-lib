#include "interface.h"

#include <span>

#include "base/io/log.h"
#include "base/text/jsonoutput.h"

#include "functionwrapper.h"
#include "interfacejs.h"
#include "jscriptcanvas.h"

using namespace Util;
using namespace Vizzu;

Interface Interface::instance;

Interface::Interface() : versionStr(std::string(Main::version))
{
	needsUpdate = false;
	logging = false;
	eventParam = nullptr;
}

const char *Interface::version() const { return versionStr.c_str(); }

void *Interface::storeChart()
{
	auto snapshot =
	    std::make_shared<Snapshot>(chart->getOptions(),
	        chart->getStyles());
	return objects.reg(snapshot);
}

void Interface::restoreChart(void *chartPtr)
{
	auto snapshot = objects.get<Snapshot>(chartPtr);
	chart->setOptions(snapshot->options);
	chart->setStyles(snapshot->styles);
}

void *Interface::storeAnim()
{
	auto animation = chart->getAnimation();
	auto anim = std::make_shared<Animation>(animation,
	    Snapshot(chart->getOptions(),
	        chart->getStyles()));

	return objects.reg(anim);
}

void Interface::restoreAnim(void *animPtr)
{
	auto anim = objects.get<Animation>(animPtr);
	chart->setAnimation(anim->animation);
	chart->setOptions(anim->snapshot.options);
	chart->setStyles(anim->snapshot.styles);
}

void Interface::freeObj(void *ptr) { objects.unreg(ptr); }

const char *Interface::getStyleList()
{
	static std::string res = Text::toJSon(Styles::Sheet::paramList());
	return res.c_str();
}

const char *Interface::getStyleValue(const char *path, bool computed)
{
	if (chart) {
		static std::string res;
		auto &styles = computed
		                 ? chart->getComputedStyles()
		                 : chart->getStyles();
		res = Styles::Sheet::getParam(styles, path);
		return res.c_str();
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::setStyleValue(const char *path, const char *value)
{
	if (chart) {
		chart->getStylesheet().setParams(path, value);
	}
	else
		throw std::logic_error("No chart exists");
}

const char *Interface::getChartParamList()
{
	static std::string res = Text::toJSon(Gen::Config::listParams());
	return res.c_str();
}

const char *Interface::getChartValue(const char *path)
{
	if (chart) {
		static std::string res;
		res = chart->getConfig().getParam(path);
		return res.c_str();
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::setChartValue(const char *path, const char *value)
{
	try {
		if (chart) {
			chart->getConfig().setParam(path, value);
		}
		else
			throw std::logic_error("No chart exists");
	}
	catch (std::exception &e) {
		throw std::logic_error(
		    std::string(path) + "/" + value + ": " + e.what());
	}
}

void Interface::relToCanvasCoords(double rx,
    double ry,
    double &x,
    double &y)
{
	if (chart) {
		Geom::Point from(rx, ry);
		auto to = chart->getCoordSystem().convert(from);
		x = to.x;
		y = to.y;
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::canvasToRelCoords(double x,
    double y,
    double &rx,
    double &ry)
{
	if (chart) {
		Geom::Point from(x, y);
		auto to =
		    chart->getCoordSystem().getOriginal(from);
		rx = to.x;
		ry = to.y;
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::setChartFilter(bool (*filter)(const void *))
{
	if (chart) {
		auto wrappedFilter =
		    FunctionWrapper<bool, Data::RowWrapper>::wrap(filter);
		chart->getConfig().setFilter(wrappedFilter,
		    (intptr_t)filter);
	}
}

const void *Interface::getRecordValue(void *record,
    const char *column,
    bool isDimension)
{
	auto &row = *static_cast<const Data::RowWrapper *>(record);
	auto cell = row[column];
	if (isDimension)
		return static_cast<const void *>(cell.dimensionValue());
	else
		return static_cast<const void *>(&(*cell));
}

int Interface::addEventListener(const char *event)
{
	auto &ed = chart->getEventDispatcher();
	auto id = ed[event]->attach(
	    [&](EventDispatcher::Params &params)
	    {
		    eventParam = &params;
		    auto jsonStrIn = params.toJsonString();
		    event_invoked(params.handler, jsonStrIn.c_str());
		    eventParam = nullptr;
	    });
	return (int)id;
}

void Interface::removeEventListener(const char *event, int id)
{
	auto &ed = chart->getEventDispatcher();
	ed[event]->detach(id);
}

void Interface::preventDefaultEvent()
{
	if (eventParam) eventParam->preventDefault = true;
}

void Interface::animate(void (*callback)(bool))
{
	if (chart)
		chart->animate(
		    [=](bool ok)
		    {
			    callback(ok);
		    });
	else
		throw std::logic_error("No chart exists");
}

void Interface::setKeyframe()
{
	if (chart)
		chart->setKeyframe();
	else
		throw std::logic_error("No chart exists");
}

const char *Interface::getMarkerData(unsigned id)
{
	if (chart && chart->getPlot()) {
		static std::string res;
		const auto *marker = chart->markerByIndex(id);
		if (marker)
			res = marker->toJson(
			    chart->getPlot()->getTable());
		return res.c_str();
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::animControl(const char *command, const char *param)
{
	if (chart) {
		auto &ctrl = chart->getAnimControl();
		std::string cmd(command);
		if (cmd == "seek")
			ctrl.seek(param);
		else if (cmd == "pause")
			ctrl.pause();
		else if (cmd == "play")
			ctrl.play();
		else if (cmd == "stop")
			ctrl.stop();
		else if (cmd == "cancel")
			ctrl.cancel();
		else if (cmd == "reverse")
			ctrl.reverse();
		else
			throw std::logic_error("invalid animation command");
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::setAnimValue(const char *path, const char *value)
{
	if (chart) {
		chart->getAnimOptions().set(path, value);
	}
}

void Interface::addDimension(const char *name,
    const char **categories,
    int count)
{
	if (chart && categories) {
		std::span<const char *> view(categories, count);
		auto &table = chart->getTable();
		table.addColumn(name, view);
	}
}

void Interface::addMeasure(const char *name,
    double *values,
    int count)
{
	if (chart) {
		std::span<double> view(values, count);
		auto &table = chart->getTable();
		table.addColumn(name, view);
	}
}

void Interface::addRecord(const char **cells, int count)
{
	if (chart) {
		std::span<const char *> view(cells, count);
		auto &table = chart->getTable();
		table.pushRow(view);
	}
}

const char *Interface::dataMetaInfo()
{
	if (chart) {
		static std::string res;
		res.clear();
		auto &table = chart->getTable();
		res += "[";
		for (auto i = 0u; i < table.columnCount(); ++i) {
			res += table.getInfo(Data::ColumnIndex(i)).toJSon();
			if (i < table.columnCount() - 1) res += ",";
		}
		res += "]";
		return res.c_str();
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::init()
{
	IO::Log::set(
	    [=, this](const std::string &msg)
	    {
		    if (logging) log((msg + "\n").c_str());
	    });

	taskQueue = std::make_shared<GUI::TaskQueue>();
	auto&& chartWidget = std::make_shared<UI::ChartWidget>(taskQueue);
	chart = {chartWidget, std::addressof(chartWidget->getChart())};

	chartWidget->doChange = [&]
	{
		needsUpdate = true;
	};
	chartWidget->doSetCursor = [&](GUI::Cursor cursor)
	{
		::setCursor(toCSS(cursor));
	};
	chartWidget->openUrl = [&](const std::string &url)
	{
		::openUrl(url.c_str());
	};
	widget = std::move(chartWidget);
	needsUpdate = true;
}

void Interface::poll()
{
	if (taskQueue) taskQueue->poll();
}

void Interface::update(double width,
    double height,
    RenderControl renderControl)
{
	if (!chart) throw std::logic_error("No chart exists");

	auto now = std::chrono::steady_clock::now();
	chart->getAnimControl().update(now);

	Geom::Size size(width, height);

	bool renderNeeded = needsUpdate || widget->getSize() != size;

	if ((renderControl == allow && renderNeeded)
	    || renderControl == force) {
		Vizzu::Main::JScriptCanvas canvas;
		canvas.frameBegin();
		widget->onUpdateSize(canvas, size);
		widget->onDraw(canvas);
		canvas.frameEnd();
		needsUpdate = false;
	}
}

void Interface::pointerDown(int pointerId, double x, double y)
{
	if (widget) {
		widget->onPointerDown(
		    GUI::PointerEvent(pointerId, Geom::Point(x, y)));
		needsUpdate = true;
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::pointerUp(int pointerId, double x, double y)
{
	if (widget) {
		widget->onPointerUp(
		    GUI::PointerEvent(pointerId, Geom::Point(x, y)));
		needsUpdate = true;
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::pointerLeave(int pointerId)
{
	if (widget) {
		widget->onPointerLeave(
		    GUI::PointerEvent(pointerId, Geom::Point::Invalid()));
		needsUpdate = true;
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::wheel(double delta)
{
	if (widget) {
		widget->onWheel(delta);
		needsUpdate = true;
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::pointerMove(int pointerId, double x, double y)
{
	if (widget) {
		widget->onPointerMove(
		    GUI::PointerEvent(pointerId, Geom::Point(x, y)));
		needsUpdate = true;
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::keyPress(int key, bool ctrl, bool alt, bool shift)
{
	if (widget) {
		GUI::KeyModifiers keyModifiers(shift, ctrl, alt);
		widget->onKeyPress(GUI::Key(key), keyModifiers);
		needsUpdate = true;
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::log(const char *str) { jsconsolelog(str); }
