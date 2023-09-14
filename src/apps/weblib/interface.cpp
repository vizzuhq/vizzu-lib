#include "interface.h"

#include <span>

#include "base/conv/auto_json.h"
#include "base/io/log.h"

#include "interfacejs.h"
#include "jscriptcanvas.h"
#include "jsfunctionwrapper.h"

using namespace Util;
using namespace Vizzu;

template <class T, class Deleter>
std::unique_ptr<T, Deleter> create_unique_ptr(T *&&ptr,
    Deleter &&deleter)
{
	return {ptr, std::forward<Deleter>(deleter)};
}

Interface &Interface::getInstance()
{
	static Interface instance;
	return instance;
};

Interface::Interface()
{
	IO::Log::setEnabled(false);
	IO::Log::setTimestamp(false);
}

const char *Interface::version()
{
	static const std::string versionStr{Main::version};
	return versionStr.c_str();
}

ObjectRegistry::Handle Interface::storeChart()
{
	return objects.reg(std::make_shared<Snapshot>(chart->getOptions(),
	    chart->getStyles()));
}

void Interface::restoreChart(ObjectRegistry::Handle chartPtr)
{
	auto &&snapshot = objects.get<Snapshot>(chartPtr);
	chart->setOptions(snapshot->options);
	chart->setStyles(snapshot->styles);
}

ObjectRegistry::Handle Interface::storeAnim()
{
	return objects.reg(
	    std::make_shared<Animation>(chart->getAnimation(),
	        Snapshot(chart->getOptions(), chart->getStyles())));
}

void Interface::restoreAnim(ObjectRegistry::Handle animPtr)
{
	auto &&anim = objects.get<Animation>(animPtr);
	chart->setAnimation(anim->animation);
	chart->setOptions(anim->snapshot.options);
	chart->setStyles(anim->snapshot.styles);
}

void Interface::freeObj(ObjectRegistry::Handle ptr)
{
	objects.unreg(ptr);
}

const char *Interface::getStyleList()
{
	static const std::string res =
	    Conv::toJSON(Styles::Sheet::paramList());
	return res.c_str();
}

const char *Interface::getStyleValue(const char *path, bool computed)
{
	if (chart) {
		thread_local std::string res;
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
		thread_local std::string res;
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
		chart->getConfig().setFilter(
		    Data::Filter::Function{std::move(filter)},
		    hash);
	}
}

std::variant<const char *, double> Interface::getRecordValue(
    const Data::RowWrapper &record,
    const char *column)
{
	auto cell = record[column];
	if (cell.isDimension()) return cell.dimensionValue();

	return *cell;
}

void Interface::addEventListener(const char *event,
    void (*callback)(ObjectRegistry::Handle, const char *))
{
	if (auto &&ev = chart->getEventDispatcher().getEvent(event)) {
		ev->attach(std::hash<decltype(callback)>{}(callback),
		    [this, callback](EventDispatcher::Params &params)
		    {
			    auto &&jsonStrIn = params.toJSON();

			    callback(create_unique_ptr(
			                 objects.reg<EventDispatcher::Params>(
			                     {std::shared_ptr<void>{}, &params}),
			                 [this](const void *handle)
			                 {
				                 objects.unreg(handle);
			                 })
			                 .get(),
			        jsonStrIn.c_str());
		    });
	}
}

void Interface::removeEventListener(const char *event,
    void (*callback)(ObjectRegistry::Handle, const char *))
{
	if (auto &&ev = chart->getEventDispatcher().getEvent(event)) {
		ev->detach(std::hash<decltype(callback)>{}(callback));
	}
}

void Interface::preventDefaultEvent(ObjectRegistry::Handle obj)
{
	objects.get<EventDispatcher::Params>(obj)->preventDefault = true;
}

void Interface::animate(void (*callback)(bool))
{
	if (chart)
		chart->animate(callback);
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
		thread_local std::string res;
		if (const auto *marker = chart->markerByIndex(id))
			res = marker->toJSON();
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
		thread_local std::string res;
		res = Conv::toJSON(chart->getTable().getInfos());
		return res.c_str();
	}
	throw std::logic_error("No chart exists");
}

ObjectRegistry::Handle Interface::createChart()
{
	widget = std::make_shared<UI::ChartWidget>(scheduler);
	chart = {widget, std::addressof(widget->getChart())};

	widget->doSetCursor = [&](GUI::Cursor cursor)
	{
		::setCursor(toCSS(cursor));
	};
	widget->openUrl = [&](const std::string &url)
	{
		::openUrl(url.c_str());
	};

	return objects.reg(std::static_pointer_cast<GUI::Widget>(widget));
}

void Interface::setLogging(bool enable)
{
	IO::Log::setEnabled(enable);
}

void Interface::update(double width,
    double height,
    RenderControl renderControl)
{
	if (!chart) throw std::logic_error("No chart exists");

	auto now = std::chrono::steady_clock::now();
	chart->getAnimControl().update(now);

	const Geom::Size size{width, height};

	const bool renderNeeded =
	    widget->needsUpdate() || widget->getSize() != size;

	if ((renderControl == allow && renderNeeded)
	    || renderControl == force) {
		Vizzu::Main::JScriptCanvas canvas;
		canvas.frameBegin();
		widget->onUpdateSize(canvas, size);
		widget->onDraw(canvas);
		canvas.frameEnd();
	}
}

void Interface::pointerDown(int pointerId, double x, double y)
{
	if (widget) {
		widget->onPointerDown(
		    GUI::PointerEvent(pointerId, Geom::Point{x, y}));
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::pointerUp(int pointerId, double x, double y)
{
	if (widget) {
		widget->onPointerUp(
		    GUI::PointerEvent(pointerId, Geom::Point{x, y}));
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::pointerLeave(int pointerId)
{
	if (widget) {
		widget->onPointerLeave(
		    GUI::PointerEvent(pointerId, Geom::Point::Invalid()));
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::wheel(double delta)
{
	if (widget) { widget->onWheel(delta); }
	else
		throw std::logic_error("No chart exists");
}

void Interface::pointerMove(int pointerId, double x, double y)
{
	if (widget) {
		widget->onPointerMove(
		    GUI::PointerEvent(pointerId, Geom::Point{x, y}));
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::keyPress(int key, bool ctrl, bool alt, bool shift)
{
	if (widget) {
		const GUI::KeyModifiers keyModifiers(shift, ctrl, alt);
		widget->onKeyPress(static_cast<GUI::Key>(key), keyModifiers);
	}
	else
		throw std::logic_error("No chart exists");
}

void Interface::CScheduler::schedule(const Task &task,
    std::chrono::steady_clock::time_point time)
{
	auto it = [&, lock = std::lock_guard{mutex}]
	{
		return tasks.emplace(tasks.end(), task, this);
	}();
	it->it = it;

	::callLater(
	    [](void *task)
	    {
		    create_unique_ptr(static_cast<ScheduledTask *>(task),
		        [](ScheduledTask *task)
		        {
			        auto lock =
			            std::lock_guard{task->scheduler->mutex};
			        task->scheduler->tasks.erase(task->it);
		        })
		        ->task();
	    },
	    std::to_address(it),
	    static_cast<int>(
	        (time - std::chrono::steady_clock::now()).count()));
}