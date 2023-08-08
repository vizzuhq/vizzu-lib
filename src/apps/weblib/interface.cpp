#include "interface.h"

#include <span>

#include "base/conv/auto_json.h"
#include "base/io/log.h"

#include "interfacejs.h"
#include "jscriptcanvas.h"
#include "jsfunctionwrapper.h"

using namespace Util;
using namespace Vizzu;

Interface Interface::instance;

Interface::Interface() : versionStr(std::string(Main::version))
{
	IO::Log::setEnabled(false);
	IO::Log::setTimestamp(false);
	needsUpdate = false;
	eventParam = nullptr;
}

const char *Interface::version() const { return versionStr.c_str(); }

void *Interface::storeChart()
{
	auto snapshot = std::make_shared<Snapshot>(chart->getOptions(),
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
	    Snapshot(chart->getOptions(), chart->getStyles()));

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
	static const std::string res =
	    Conv::toJSON(Styles::Sheet::paramList());
	return res.c_str();
}

const char *Interface::getStyleValue(const char *path, bool computed)
{
	if (chart) {
		static std::string res;
		auto &styles = computed ? chart->getComputedStyles()
		                        : chart->getStyles();
		res = Styles::Sheet::getParam(styles, path);
		return res.c_str();
	}
	throw std::logic_error("No chart exists");
}

void Interface::setStyleValue(const char *path, const char *value)
{
	if (chart) { chart->getStylesheet().setParams(path, value); }
	else
		throw std::logic_error("No chart exists");
}

const char *Interface::getChartParamList()
{
	static const std::string res =
	    Conv::toJSON(Gen::Config::listParams());
	return res.c_str();
}

const char *Interface::getChartValue(const char *path)
{
	if (chart) {
		static std::string res;
		res = chart->getConfig().getParam(path);
		return res.c_str();
	}
	throw std::logic_error("No chart exists");
}

void Interface::setChartValue(const char *path, const char *value)
{
	if (chart)
		chart->getConfig().setParam(path, value);
	else
		throw std::logic_error("No chart exists");
}

void Interface::relToCanvasCoords(double rx,
    double ry,
    double &x,
    double &y)
{
	if (chart) {
		const Geom::Point from(rx, ry);
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
		const Geom::Point from(x, y);
		auto to = chart->getCoordSystem().getOriginal(from);
		rx = to.x;
		ry = to.y;
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::setChartFilter(
    JsFunctionWrapper<bool, const Data::RowWrapper &> &&filter)
{
	if (chart) {
		const auto hash = filter.hash();
		chart->getConfig().setFilter(std::move(filter), hash);
	}
}

const void *Interface::getRecordValue(void *record,
    const char *column,
    bool isDimension)
{
	const auto &row = *static_cast<const Data::RowWrapper *>(record);
	auto cell = row[column];
	if (isDimension)
		return static_cast<const void *>(cell.dimensionValue());

	return static_cast<const void *>(&(*cell));
}

void Interface::addEventListener(const char *event,
    void (*callback)(const char *))
{
	if (auto &&ev = chart->getEventDispatcher().getEvent(event)) {
		ev->attach(std::hash<void (*)(const char *)>{}(callback),
		    [this, callback](EventDispatcher::Params &params)
		    {
			    eventParam = &params;
			    auto jsonStrIn = params.toJsonString();
			    callback(jsonStrIn.c_str());
			    eventParam = nullptr;
		    });
	}
}

void Interface::removeEventListener(const char *event,
    void (*callback)(const char *))
{
	if (auto &&ev = chart->getEventDispatcher().getEvent(event)) {
		ev->detach(std::hash<void (*)(const char *)>{}(callback));
	}
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
			res = marker->toJson(chart->getPlot()->getTable());
		return res.c_str();
	}
	throw std::logic_error("No chart exists");
}

void Interface::animControl(const char *command, const char *param)
{
	if (chart) {
		auto &ctrl = chart->getAnimControl();
		const std::string cmd(command);
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
	if (chart) { chart->getAnimOptions().set(path, value); }
}

void Interface::addDimension(const char *name,
    const char **categories,
    int count)
{
	if (chart && categories) {
		auto &table = chart->getTable();
		table.addColumn(name,
		    {categories, static_cast<size_t>(count)});
	}
}

void Interface::addMeasure(const char *name,
    double *values,
    int count)
{
	if (chart) {
		auto &table = chart->getTable();
		table.addColumn(name, {values, static_cast<size_t>(count)});
	}
}

void Interface::addRecord(const char **cells, int count)
{
	if (chart) {
		auto &table = chart->getTable();
		table.pushRow({cells, static_cast<size_t>(count)});
	}
}

const char *Interface::dataMetaInfo()
{
	if (chart) {
		static std::string res;
		res = Conv::toJSON(chart->getTable().getInfos());
		return res.c_str();
	}
	throw std::logic_error("No chart exists");
}

void Interface::init()
{
	taskQueue = std::make_shared<GUI::TaskQueue>();
	auto &&chartWidget = std::make_shared<UI::ChartWidget>(taskQueue);
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

void Interface::setLogging(bool enable)
{
	IO::Log::setEnabled(enable);
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

	const Geom::Size size(width, height);

	const bool renderNeeded =
	    needsUpdate || widget->getSize() != size;

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
		const GUI::KeyModifiers keyModifiers(shift, ctrl, alt);
		widget->onKeyPress(GUI::Key(key), keyModifiers);
		needsUpdate = true;
	}
	else
		throw std::logic_error("No chart exists");
}
