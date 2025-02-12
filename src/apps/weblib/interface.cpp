#include "interface.h"

#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <ratio>
#include <span>
#include <stdexcept>
#include <string>
#include <variant>

#include "base/anim/duration.h"
#include "base/conv/auto_json.h"
#include "base/io/log.h"
#include "base/refl/auto_accessor.h"
#include "chart/animator/animation.h"
#include "chart/generator/plotptr.h"
#include "chart/main/style.h"
#include "chart/main/stylesheet.h"
#include "chart/main/version.h"
#include "chart/options/config.h"
#include "chart/options/options.h"
#include "chart/ui/chart.h"
#include "dataframe/old/types.h"

#include "cinterface.h"
#include "interfacejs.h"
#include "jscriptcanvas.h"
#include "jswrappers.h"
#include "objectregistry.h"

namespace Vizzu
{

template <class T, class Deleter>
std::unique_ptr<T, Deleter> create_unique_ptr(T *&&ptr,
    Deleter &&deleter)
{
	return {std::move(ptr), std::forward<Deleter>(deleter)};
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
    ObjectRegistryHandle chart)
{
	auto &&widget = objects.get<UI::ChartWidget>(chart);
	auto &chartRef = widget->getChart();
	return {std::move(widget), &chartRef};
}

ObjectRegistryHandle Interface::storeChart(ObjectRegistryHandle chart)
{
	auto &&chartPtr = getChart(chart);
	return objects.reg(
	    std::make_shared<Snapshot>(chartPtr->getOptions(),
	        chartPtr->getStyles()));
}

void Interface::restoreChart(ObjectRegistryHandle chart,
    ObjectRegistryHandle snapshot)
{
	auto &&snapshotPtr = objects.get<Snapshot>(snapshot);
	auto &&chartPtr = getChart(chart);
	chartPtr->setOptions(snapshotPtr->options);
	chartPtr->setStyles(snapshotPtr->styles);
}

ObjectRegistryHandle Interface::storeAnim(ObjectRegistryHandle chart)
{
	auto &&chartPtr = getChart(chart);
	return objects.reg(
	    std::make_shared<Animation>(chartPtr->getAnimation(),
	        Snapshot(chartPtr->getOptions(), chartPtr->getStyles())));
}

void Interface::restoreAnim(ObjectRegistryHandle chart,
    ObjectRegistryHandle animPtr)
{
	auto &&anim = objects.get<Animation>(animPtr);
	auto &&chartPtr = getChart(chart);
	chartPtr->setAnimation(anim->animation);
	chartPtr->setOptions(anim->snapshot.options);
	chartPtr->setStyles(anim->snapshot.styles);
}

void Interface::freeObj(ObjectRegistryHandle ptr)
{
	objects.unreg(ptr);
}

const char *Interface::getStyleList()
{
	static const std::string res =
	    Conv::toJSON(Styles::Sheet::listParams());
	return res.c_str();
}

const char *Interface::getStyleValue(ObjectRegistryHandle chart,
    const char *path,
    bool computed)
{
	auto &&chartPtr = getChart(chart);
	thread_local std::string res;
	const auto &styles = computed ? chartPtr->getComputedStyles()
	                              : chartPtr->getStyles();
	res = Styles::Sheet::getParam(styles, path);
	return res.c_str();
}

void Interface::setStyleValue(ObjectRegistryHandle chart,
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

const char *Interface::getChartValue(ObjectRegistryHandle chart,
    const char *path)
{
	thread_local std::string res;
	res = getChart(chart)->getConfig().getParam(path);
	return res.c_str();
}

void Interface::setChartValue(ObjectRegistryHandle chart,
    const char *path,
    const char *value)
{
	getChart(chart)->getConfig().setParam(path, value);
}

void Interface::relToCanvasCoords(ObjectRegistryHandle chart,
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

void Interface::canvasToRelCoords(ObjectRegistryHandle chart,
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

void Interface::setChartFilter(ObjectRegistryHandle chart,
    JsFunctionWrapper<bool, const Data::RowWrapper &> &&filter)
{
	if (filter)
		getChart(chart)->getOptions().dataFilter =
		    Data::Filter{std::move(filter)};
	else
		getChart(chart)->getOptions().dataFilter = {};
}

std::variant<double, const std::string *> Interface::getRecordValue(
    const Data::RowWrapper &record,
    const char *column)
{
	return record.get_value(column);
}

std::shared_ptr<Data::DataTable> Interface::getTable(
    ObjectRegistryHandle data)
{
	return objects.get<Data::DataTable>(data);
}

void Interface::addEventListener(ObjectRegistryHandle chart,
    const char *event,
    void (*callback)(APIHandles::Event, const char *))
{
	auto &&chartPtr = getChart(chart);
	if (auto &&ev = chartPtr->getEventDispatcher().getEvent(event)) {
		ev->attach(
		    [callback](Util::EventDispatcher::Params &params,
		        const std::string &jsonStrIn)
		    {
			    callback(&params, jsonStrIn.c_str());
		    },
		    std::hash<decltype(callback)>{}(callback));
	}
}

void Interface::removeEventListener(ObjectRegistryHandle chart,
    const char *event,
    void (*callback)(APIHandles::Event, const char *))
{
	auto &&chartPtr = getChart(chart);
	if (auto &&ev = chartPtr->getEventDispatcher().getEvent(event)) {
		ev->detach(std::hash<decltype(callback)>{}(callback));
	}
}

void Interface::preventDefaultEvent(APIHandles::Event obj)
{
	obj->preventDefault = true;
}

void Interface::animate(ObjectRegistryHandle chart,
    void (*callback)(bool))
{
	getChart(chart)->animate(
	    {[callback](const Gen::PlotPtr &, const bool &ok)
	        {
		        callback(ok);
	        }});
}

void Interface::setKeyframe(ObjectRegistryHandle chart)
{
	getChart(chart)->setKeyframe();
}

void Interface::setAnimControlValue(ObjectRegistryHandle chart,
    std::string_view path,
    const char *value)
{
	auto &&chartPtr = getChart(chart);
	auto &&ctrl = chartPtr->getAnimControl();

	if (path == "seek") { ctrl->seek(value); }
	else if (path == "cancel") {
		ctrl->cancel();
	}
	else if (path == "stop") {
		ctrl->stop();
	}
	else if (auto &&set_accessor =
	             Refl::Access::getAccessor<::Anim::Control::Option>(
	                 path)
	                 .set) {
		set_accessor(ctrl->getOptions(), value);
	}
	else {
		throw std::logic_error("invalid animation command");
	}
	ctrl->update();
}

const char *Interface::getAnimControlValue(ObjectRegistryHandle chart,
    std::string_view path)
{
	thread_local std::string res;

	auto &&chartPtr = getChart(chart);
	auto &&ctrl = chartPtr->getAnimControl();

	if (auto &&get_accessor =
	        Refl::Access::getAccessor<::Anim::Control::Option>(path)
	            .get) {
		res = get_accessor(ctrl->getOptions());
	}
	else
		throw std::logic_error("invalid animation command");

	return res.c_str();
}

void Interface::setAnimValue(ObjectRegistryHandle chart,
    const char *path,
    const char *value)
{
	getChart(chart)->getAnimOptions().set(path, value);
}

void Interface::addDimension(ObjectRegistryHandle table,
    const char *name,
    const char *const *categories,
    std::uint32_t categoriesCount,
    const std::uint32_t *categoryIndices,
    std::uint32_t categoryIndicesCount,
    bool isContiguous)
{
	getTable(table)->add_dimension({categories, categoriesCount},
	    {categoryIndices, categoryIndicesCount},
	    name,
	    {{{"isContiguous", isContiguous ? "true" : "false"}}});
}

void Interface::addMeasure(ObjectRegistryHandle table,
    const char *name,
    const char *unit,
    const double *values,
    std::uint32_t count)
{
	getTable(table)->add_measure({values, count},
	    name,
	    {{std::pair{"unit", unit}}});
}

void Interface::addRecord(ObjectRegistryHandle table,
    const char *const *cells,
    std::uint32_t count)
{
	getTable(table)->add_record({cells, count});
}

const char *Interface::dataMetaInfo(ObjectRegistryHandle table)
{
	thread_local std::string res;
	res = getTable(table)->as_string();
	return res.c_str();
}

struct IntFDataTable final : Data::DataTable
{
	JsCompositionWrapper<Interface::ExternalData> externalData;

	[[nodiscard]] explicit IntFDataTable(
	    JsCompositionWrapper<Interface::ExternalData>
	        &&external_data) :
	    externalData(std::move(external_data))
	{}

	[[nodiscard]] dataframe::series_meta_t get_series_meta(
	    const std::string &id) const & override
	{
		return {id,
		    externalData.values.seriesMeta(id.c_str())
		        ? dataframe::series_type::dimension
		        : dataframe::series_type::measure};
	}

	[[nodiscard]] std::string_view get_series_info(
	    const std::string &id,
	    const char *key) const & override
	{
		thread_local std::string res;
		res = create_unique_ptr(
		    externalData.values.seriesInfo(id.c_str(), key),
		    externalData.values.stringDeleter)
		          .get();
		return res;
	}

	[[noreturn]] static void unsupported()
	{
		throw std::logic_error("unsupported");
	}

	[[noreturn]] void add_dimension(std::span<const char *const>,
	    std::span<const std::uint32_t>,
	    std::string_view,
	    std::span<const std::pair<const char *, const char *>>,
	    dataframe::adding_type)
	    & override
	{
		unsupported();
	}

	[[noreturn]] void add_measure(std::span<const double>,
	    std::string_view,
	    std::span<const std::pair<const char *, const char *>>,
	    dataframe::adding_type)
	    & override
	{
		unsupported();
	}

	[[noreturn]] void add_record(std::span<const char *const>)
	    & override
	{
		unsupported();
	}

	[[nodiscard]] std::string as_string() const & override
	{
		unsupported();
	}

	[[nodiscard]] std::pair<
	    std::shared_ptr<dataframe::dataframe_interface>,
	    std::map<Data::SeriesIndex, std::string>>
	aggregate(const Data::Filter &filter,
	    const std::set<Data::SeriesIndex> &aggregate_by,
	    const std::set<Data::SeriesIndex> &aggregating)
	    const & override
	{
		auto &intf = Interface::getInstance();
		auto &&res = dataframe::dataframe::create_new();
		auto &&obj = intf.createData(&res);

		std::vector<const char *> aggregateBy(aggregate_by.size());
		std::ranges::transform(aggregate_by,
		    aggregateBy.begin(),
		    [](const Data::SeriesIndex &index)
		    {
			    return index.getColIndex().c_str();
		    });

		std::vector<const char *> aggregatingCols(aggregating.size());
		std::ranges::transform(aggregating,
		    aggregatingCols.begin(),
		    [](const Data::SeriesIndex &index)
		    {
			    return index.getColIndex().c_str();
		    });

		using ArrType =
		    Refl::EnumArray<dataframe::aggregator_type, std::string>;
		static ArrType arr = std::apply(
		    [](auto &&...names)
		    {
			    return ArrType{{std::string{names}...}};
		    },
		    Refl::enum_names<dataframe::aggregator_type>);

		std::vector<const char *> aggregatingFun(aggregating.size());
		std::ranges::transform(aggregating,
		    aggregatingFun.begin(),
		    [](const Data::SeriesIndex &index) -> const char *
		    {
			    return arr[index.getAggr()].c_str();
		    });

		std::vector<
		    std::unique_ptr<const char, void (*)(const char *)>>
		    aggregatingNames;
		aggregatingNames.reserve(aggregating.size());

		{
			std::vector<const char *> aggregatingRawNames(
			    aggregating.size());

			externalData.values.aggregator(obj,
			    filter.getFun1(),
			    filter.getFun2(),
			    aggregateBy.size(),
			    aggregateBy.data(),
			    aggregating.size(),
			    aggregatingCols.data(),
			    aggregatingFun.data(),
			    aggregatingRawNames.data());

			for (auto &&ptr : std::move(aggregatingRawNames))
				aggregatingNames.emplace_back(ptr,
				    externalData.values.stringDeleter);
		}

		intf.freeObj(obj);

		std::map<Data::SeriesIndex, std::string> resMap;
		for (auto it = aggregatingNames.data();
		     auto &&agg : aggregating)
			if (auto &&ptr = *it++)
				resMap[agg] = ptr.get();
			else
				resMap[agg] =
				    Refl::enum_name<std::string>(agg.getAggr())
				    + " of " + agg.getColIndex();

		return {res, resMap};
	}
};

ObjectRegistryHandle Interface::createData(
    std::shared_ptr<dataframe::dataframe_interface> *df)
{
	return objects.reg(std::make_shared<Data::DataTableImpl>(
	    df ? *df : dataframe::dataframe::create_new()));
}

ObjectRegistryHandle Interface::createExternalData(
    JsCompositionWrapper<ExternalData> &&externalData)
{
	return objects.reg(
	    std::make_shared<IntFDataTable>(std::move(externalData)));
}

ObjectRegistryHandle Interface::createChart(ObjectRegistryHandle data)
{
	auto &&widget = std::make_shared<UI::ChartWidget>(
	    objects.get<Data::DataTable>(data));

	auto &openUrl = widget->openUrl;
	auto &doChange = widget->getChart().onChanged;

	const auto *handle = objects.reg(std::move(widget));

	openUrl.attach(
	    [handle](const std::string &url)
	    {
		    ::chart_openUrl(handle, url.c_str());
	    });

	doChange.attach(
	    [handle]
	    {
		    ::chart_doChange(handle);
	    });

	return handle;
}

ObjectRegistryHandle Interface::createCanvas()
{
	return objects.reg(std::make_shared<Main::JScriptCanvas>());
}

void Interface::setLogging(bool enable)
{
	IO::Log::setEnabled(enable);
}

void Interface::update(ObjectRegistryHandle chart, double timeInMSecs)
{
	auto &&widget = objects.get<UI::ChartWidget>(chart);

	auto &&nanoSecs =
	    std::chrono::duration_cast<std::chrono::nanoseconds>(
	        std::chrono::duration<double, std::milli>{timeInMSecs});

	widget->getChart().getAnimControl()->update(
	    ::Anim::TimePoint{nanoSecs});
}

void Interface::render(ObjectRegistryHandle chart,
    ObjectRegistryHandle canvas,
    double width,
    double height)
{
	auto &&widget = objects.get<UI::ChartWidget>(chart);
	auto &&ptr = objects.get<Gfx::ICanvas>(canvas);

	ptr->frameBegin();

	widget->onUpdateSize({width, height});

	widget->onDraw(ptr);

	ptr->frameEnd();
}

void Interface::setLineResolution(ObjectRegistryHandle canvas,
    double distanceMax,
    double curveHeightMax)
{
	static_cast<Draw::Painter *>(
	    objects.get<Gfx::ICanvas>(canvas)->getPainter())
	    ->setPathSamplerOptions({distanceMax, curveHeightMax});
}

void Interface::pointerDown(ObjectRegistryHandle chart,
    ObjectRegistryHandle,
    int pointerId,
    double x,
    double y)
{
	objects.get<UI::ChartWidget>(chart)->onPointerDown(
	    {pointerId, Geom::Point{x, y}});
}

void Interface::pointerUp(ObjectRegistryHandle chart,
    ObjectRegistryHandle,
    int pointerId,
    double x,
    double y)
{
	objects.get<UI::ChartWidget>(chart)->onPointerUp(
	    {pointerId, Geom::Point{x, y}});
}

void Interface::pointerLeave(ObjectRegistryHandle chart,
    ObjectRegistryHandle,
    int pointerId)
{
	objects.get<UI::ChartWidget>(chart)->onPointerLeave(
	    {pointerId, Geom::Point::Invalid()});
}

void Interface::wheel(ObjectRegistryHandle chart,
    ObjectRegistryHandle,
    double delta)
{
	objects.get<UI::ChartWidget>(chart)->onWheel(delta);
}

void Interface::pointerMove(ObjectRegistryHandle chart,
    ObjectRegistryHandle,
    int pointerId,
    double x,
    double y)
{
	objects.get<UI::ChartWidget>(chart)->onPointerMove(
	    {pointerId, Geom::Point{x, y}});
}

}