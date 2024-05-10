#include "interface.h"

#include <span>

#include "base/conv/auto_json.h"
#include "base/io/log.h"
#include "base/refl/auto_accessor.h"
#include "chart/main/version.h"
#include "chart/ui/chart.h"

#include "canvas.h"
#include "interfacejs.h"
#include "jscriptcanvas.h"
#include "jsfunctionwrapper.h"

namespace Vizzu
{

template <class T, class Deleter>
std::unique_ptr<T, Deleter> create_unique_ptr(T *&&ptr,
    Deleter &&deleter)
{
	return {ptr, std::forward<Deleter>(deleter)};
}

struct Interface::Snapshot
{
	Snapshot(Gen::Options options, Styles::Chart styles) :
	    options(std::move(options)),
	    styles(std::move(styles))
	{}
	Gen::Options options;
	Styles::Chart styles;
};

struct Interface::Animation
{
	Animation(Anim::AnimationPtr anim, Snapshot snapshot) :
	    animation(std::move(anim)),
	    snapshot(std::move(snapshot))
	{}
	Anim::AnimationPtr animation;
	Snapshot snapshot;
};

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

std::shared_ptr<Vizzu::Chart> Interface::getChart(
    ObjectRegistry::Handle chart)
{
	auto &&widget = objects.get<UI::ChartWidget>(chart);
	auto &chartRef = widget->getChart();
	return {std::move(widget), &chartRef};
}

ObjectRegistry::Handle Interface::storeChart(
    ObjectRegistry::Handle chart)
{
	auto &&chartPtr = getChart(chart);
	return objects.reg(
	    std::make_shared<Snapshot>(chartPtr->getOptions(),
	        chartPtr->getStyles()));
}

void Interface::restoreChart(ObjectRegistry::Handle chart,
    ObjectRegistry::Handle snapshot)
{
	auto &&snapshotPtr = objects.get<Snapshot>(snapshot);
	auto &&chartPtr = getChart(chart);
	chartPtr->setOptions(snapshotPtr->options);
	chartPtr->setStyles(snapshotPtr->styles);
}

ObjectRegistry::Handle Interface::storeAnim(
    ObjectRegistry::Handle chart)
{
	auto &&chartPtr = getChart(chart);
	return objects.reg(
	    std::make_shared<Animation>(chartPtr->getAnimation(),
	        Snapshot(chartPtr->getOptions(), chartPtr->getStyles())));
}

void Interface::restoreAnim(ObjectRegistry::Handle chart,
    ObjectRegistry::Handle animPtr)
{
	auto &&anim = objects.get<Animation>(animPtr);
	auto &&chartPtr = getChart(chart);
	chartPtr->setAnimation(anim->animation);
	chartPtr->setOptions(anim->snapshot.options);
	chartPtr->setStyles(anim->snapshot.styles);
}

void Interface::freeObj(ObjectRegistry::Handle ptr)
{
	objects.unreg(ptr);
}

const char *Interface::getStyleList()
{
	static const std::string res =
	    Conv::toJSON(Styles::Sheet::listParams());
	return res.c_str();
}

const char *Interface::getStyleValue(ObjectRegistry::Handle chart,
    const char *path,
    bool computed)
{
	auto &&chartPtr = getChart(chart);
	thread_local std::string res;
	auto &styles = computed ? chartPtr->getComputedStyles()
	                        : chartPtr->getStyles();
	res = Styles::Sheet::getParam(styles, path);
	return res.c_str();
}

void Interface::setStyleValue(ObjectRegistry::Handle chart,
    const char *path,
    const char *value)
{
	getChart(chart)->getStylesheet().setParams(path, value);
}

const char *Interface::getChartParamList()
{
	static const std::string res = Gen::Config::paramsJson();
	return res.c_str();
}

const char *Interface::getChartValue(ObjectRegistry::Handle chart,
    const char *path)
{
	thread_local std::string res;
	res = getChart(chart)->getConfig().getParam(path);
	return res.c_str();
}

void Interface::setChartValue(ObjectRegistry::Handle chart,
    const char *path,
    const char *value)
{
	getChart(chart)->getConfig().setParam(path, value);
}

void Interface::relToCanvasCoords(ObjectRegistry::Handle chart,
    double rx,
    double ry,
    double &x,
    double &y)
{
	auto to =
	    getChart(chart)->getRenderedChart().getCoordSys().convert(
	        {rx, ry});
	x = to.x;
	y = to.y;
}

void Interface::canvasToRelCoords(ObjectRegistry::Handle chart,
    double x,
    double y,
    double &rx,
    double &ry)
{
	auto to =
	    getChart(chart)->getRenderedChart().getCoordSys().getOriginal(
	        {x, y});
	rx = to.x;
	ry = to.y;
}

void Interface::setChartFilter(ObjectRegistry::Handle chart,
    JsFunctionWrapper<bool, const Data::RowWrapper &> &&filter)
{
	if (filter)
		getChart(chart)->getOptions().dataFilter =
		    Data::Filter{std::move(filter)};
	else
		getChart(chart)->getOptions().dataFilter = {};
}

std::variant<double, std::string_view> Interface::getRecordValue(
    const Data::RowWrapper &record,
    const char *column)
{
	return record.get_value(column);
}

void Interface::addEventListener(ObjectRegistry::Handle chart,
    const char *event,
    void (*callback)(ObjectRegistry::Handle, const char *))
{
	auto &&chartPtr = getChart(chart);
	if (auto &&ev = chartPtr->getEventDispatcher().getEvent(event)) {
		ev->attach(
		    [this, callback](Util::EventDispatcher::Params &params,
		        const std::string &jsonStrIn)
		    {
			    auto &&ptr = create_unique_ptr(
			        objects.reg<Util::EventDispatcher::Params>(
			            {std::shared_ptr<void>{}, &params}),
			        [this](const void *handle)
			        {
				        objects.unreg(handle);
			        });
			    callback(ptr.get(), jsonStrIn.c_str());
		    },
		    std::hash<decltype(callback)>{}(callback));
	}
}

void Interface::removeEventListener(ObjectRegistry::Handle chart,
    const char *event,
    void (*callback)(ObjectRegistry::Handle, const char *))
{
	auto &&chartPtr = getChart(chart);
	if (auto &&ev = chartPtr->getEventDispatcher().getEvent(event)) {
		ev->detach(std::hash<decltype(callback)>{}(callback));
	}
}

void Interface::preventDefaultEvent(ObjectRegistry::Handle obj)
{
	objects.get<Util::EventDispatcher::Params>(obj)->preventDefault =
	    true;
}

void Interface::animate(ObjectRegistry::Handle chart,
    void (*callback)(bool))
{
	getChart(chart)->animate(callback);
}

void Interface::setKeyframe(ObjectRegistry::Handle chart)
{
	getChart(chart)->setKeyframe();
}

void Interface::setAnimControlValue(ObjectRegistry::Handle chart,
    std::string_view path,
    const char *value)
{
	auto &&chartPtr = getChart(chart);
	auto &ctrl = chartPtr->getAnimControl();

	if (path == "seek") { ctrl.seek(value); }
	else if (path == "cancel") {
		ctrl.cancel();
	}
	else if (path == "stop") {
		ctrl.stop();
	}
	else if (auto &&set_accessor =
	             Refl::Access::getAccessor<::Anim::Control::Option>(
	                 path)
	                 .set) {
		set_accessor(ctrl.getOptions(), value);
	}
	else {
		throw std::logic_error("invalid animation command");
	}
	ctrl.update();
}

const char *Interface::getAnimControlValue(
    ObjectRegistry::Handle chart,
    std::string_view path)
{
	thread_local std::string res;

	auto &&chartPtr = getChart(chart);
	auto &ctrl = chartPtr->getAnimControl();

	if (auto &&get_accessor =
	        Refl::Access::getAccessor<::Anim::Control::Option>(path)
	            .get) {
		res = get_accessor(ctrl.getOptions());
	}
	else
		throw std::logic_error("invalid animation command");

	return res.c_str();
}

void Interface::setAnimValue(ObjectRegistry::Handle chart,
    const char *path,
    const char *value)
{
	getChart(chart)->getAnimOptions().set(path, value);
}

void Interface::addDimension(ObjectRegistry::Handle chart,
    const char *name,
    const char **categories,
    std::uint32_t categoriesCount,
    const std::uint32_t *categoryIndices,
    std::uint32_t categoryIndicesCount)
{
	if (categories) {
		getChart(chart)->getTable().addColumn(name,
		    {categories, categoriesCount},
		    {categoryIndices, categoryIndicesCount});
	}
}

void Interface::addMeasure(ObjectRegistry::Handle chart,
    const char *name,
    const char *unit,
    const double *values,
    std::uint32_t count)
{
	getChart(chart)->getTable().addColumn(name,
	    unit,
	    {values, count});
}

void Interface::addRecord(ObjectRegistry::Handle chart,
    const char **cells,
    std::uint32_t count)
{
	getChart(chart)->getTable().pushRow({cells, count});
}

const char *Interface::dataMetaInfo(ObjectRegistry::Handle chart)
{
	thread_local std::string res;
	res = getChart(chart)->getTable().getInfos();
	return res.c_str();
}

ObjectRegistry::Handle Interface::createChart()
{
	auto &&widget = std::make_shared<UI::ChartWidget>();

	auto handle = objects.reg(widget);

	widget->openUrl = [handle](const std::string &url)
	{
		::chart_openUrl(handle, url.c_str());
	};

	widget->doChange = [handle]()
	{
		::chart_doChange(handle);
	};

	return handle;
}

ObjectRegistry::Handle Interface::createCanvas()
{
	return objects.reg(
	    std::make_shared<Vizzu::Main::JScriptCanvas>());
}

void Interface::setLogging(bool enable)
{
	IO::Log::setEnabled(enable);
}

void Interface::update(ObjectRegistry::Handle chart,
    double timeInMSecs)
{
	auto &&widget = objects.get<UI::ChartWidget>(chart);

	std::chrono::duration<double, std::milli> milliSecs(timeInMSecs);

	auto nanoSecs =
	    std::chrono::duration_cast<std::chrono::nanoseconds>(
	        milliSecs);

	::Anim::TimePoint time(nanoSecs);

	widget->getChart().getAnimControl().update(time);
}

void Interface::render(ObjectRegistry::Handle chart,
    ObjectRegistry::Handle canvas,
    double width,
    double height)
{
	auto &&widget = objects.get<UI::ChartWidget>(chart);
	auto &&ptr = objects.get<Vizzu::Main::JScriptCanvas>(canvas);

	ptr->frameBegin();

	widget->onUpdateSize({width, height});

	widget->onDraw(ptr);

	ptr->frameEnd();
}

void Interface::pointerDown(ObjectRegistry::Handle chart,
    ObjectRegistry::Handle,
    int pointerId,
    double x,
    double y)
{
	objects.get<UI::ChartWidget>(chart)->onPointerDown(
	    {pointerId, Geom::Point{x, y}});
}

void Interface::pointerUp(ObjectRegistry::Handle chart,
    ObjectRegistry::Handle,
    int pointerId,
    double x,
    double y)
{
	objects.get<UI::ChartWidget>(chart)->onPointerUp(
	    {pointerId, Geom::Point{x, y}});
}

void Interface::pointerLeave(ObjectRegistry::Handle chart,
    ObjectRegistry::Handle,
    int pointerId)
{
	objects.get<UI::ChartWidget>(chart)->onPointerLeave(
	    {pointerId, Geom::Point::Invalid()});
}

void Interface::wheel(ObjectRegistry::Handle chart,
    ObjectRegistry::Handle,
    double delta)
{
	objects.get<UI::ChartWidget>(chart)->onWheel(delta);
}

void Interface::pointerMove(ObjectRegistry::Handle chart,
    ObjectRegistry::Handle,
    int pointerId,
    double x,
    double y)
{
	objects.get<UI::ChartWidget>(chart)->onPointerMove(
	    {pointerId, Geom::Point{x, y}});
}

}