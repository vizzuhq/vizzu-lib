#include "config.h"

#include "base/io/log.h"
#include "base/conv/parse.h"
#include "base/text/smartstring.h"
#include "base/text/jsonoutput.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

const Config::Accessors Config::accessors = Config::initAccessors();

std::list<std::string>  Config::listParams()
{
	std::list<std::string> res;
	for (const auto &accessor : accessors) res.push_back(accessor.first);

	auto channelParams = listChannelParams();
	for (auto id = 0u; id < ScaleId::EnumInfo::count(); id++)
	{
		auto scaleName = std::string((ScaleId)id);
		for (auto &param: channelParams) 
			res.push_back("channels." + scaleName + "." + param);
	}

	return res;
}

void Config::setParam(
	const std::string &path,
    const std::string &value)
{
	if (Text::SmartString::startsWith(path, "channels."))
	{
		setChannelParam(path, value);
	}
	else
	{
		auto it = accessors.find(path);
		if (it == accessors.end()) 
			throw std::logic_error("invalid config parameter: " + path);
		it->second.set(*setter, value);
	}
}

std::string Config::getParam(const std::string &path) const
{
	if (Text::SmartString::startsWith(path, "channels."))
	{
		return getChannelParam(path);
	}
	else
	{
		auto it = accessors.find(path);
		if (it == accessors.end()) 
			throw std::logic_error("invalid config parameter: " + path);
		return it->second.get(setter->getOptions());
	}
}

void Config::setFilter(Filter filter, ReleaseFilter deleter)
{
	struct Releaser {
		Releaser(Filter filter, ReleaseFilter deleter) : 
			filter(filter),
			deleter(deleter) 
		{}
		~Releaser() {
			deleter(filter);
		}
		Filter filter;
		ReleaseFilter deleter;
	};

	Data::Filter::Function func;

	if (filter)
	{
		std::shared_ptr<Releaser> releaser;
		if (deleter) {
			releaser = std::make_shared<Releaser>(filter, deleter);
		}

		func = [&, filter, releaser](const Data::RowWrapper &row)
		{
			return filter(static_cast<const void *>(&row));
		};
	}
	setter->setFilter(func);
}

void Config::setChannelParam(
	const std::string &path,
	const std::string &value)
{
	auto parts = Text::SmartString::split(path, '.');
	auto id = ScaleId(ScaleId(parts.at(1)));
	auto property = parts.at(2);

	if (property == "title") {
		setter->setTitle(id, value);
	}
	else if (property == "axis") 
	{
		setter->setAxisLine(id, Conv::parse<bool>(value));
	}
	else if (property == "ticks") 
	{
		setter->setTicks(id, Conv::parse<bool>(value));
	}
	else if (property == "interlacing") 
	{
		setter->setInterlacing(id, Conv::parse<bool>(value));
	}
	else if (property == "guides") 
	{
		setter->setGuides(id, Conv::parse<bool>(value));
	}
	else if (property == "attach")
	{
		setter->addSeries(id, value);
	}
	else if (property == "detach")
	{
		setter->deleteSeries(id, value);
	}
	else if (property == "set")
	{
		if (parts.size() == 3 && value == "null")
			setter->clearSeries(id);
		else {
			if (std::stoi(parts.at(3)) == 0) setter->clearSeries(id);
			setter->addSeries(id, value);
		}
	}
	else if (property == "stackable")
	{
		setter->setStackable(id, Conv::parse<bool>(value));
	}
	else if (property == "range")
	{
		if (parts.size() >= 4 && parts.at(3) == "min")
		{
			setter->setRangeMin(id, Conv::parse<OptionalScaleExtrema>(value));
		}
		else if (parts.size() >= 4 && parts.at(3) == "max")
		{
			setter->setRangeMax(id, Conv::parse<OptionalScaleExtrema>(value));
		}
		else throw std::logic_error("invalid range setting");
	}
	else if (property == "labelLevel")
	{
		setter->setLabelLevel(id, Conv::parse<uint64_t>(value));
	}
	else throw std::logic_error("invalid channel parameter: " + property);
}

std::string Config::getChannelParam(const std::string &path) const
{
	auto parts = Text::SmartString::split(path, '.');
	auto id = ScaleId(ScaleId(parts.at(1)));
	auto property = parts.at(2);

	auto &scale = setter->getOptions().getScales().at(id);

	if (property == "title") {
		return Conv::toString(scale.title.get());
	}
	else if (property == "axis") {
		return Conv::toString(scale.axisLine.get());
	}
	else if (property == "ticks") {
		return Conv::toString(scale.ticks.get());
	}
	else if (property == "interlacing") {
		return Conv::toString(scale.interlacing.get());
	}
	else if (property == "guides") {
		return Conv::toString(scale.guides.get());
	}
	else if (property == "set")
	{
		auto list = scale.discreteNames(*setter->getTable());
		auto continous = scale.continousName(*setter->getTable());
		if (!continous.empty()) list.push_front(continous);
		return Text::toJSon(list);
	}
	else if (property == "stackable")
	{
		return Conv::toString(scale.stackable());
	}
	else if (property == "range")
	{
		if (parts.size() == 4 && parts.at(3) == "min")
		{
			return Conv::toString(scale.range.get().min);
		}
		else if (parts.size() == 4 && parts.at(3) == "max")
		{
			return Conv::toString(scale.range.ref().max);
		}
		else throw std::logic_error("invalid range parameter: " + path);
	}
	else if (property == "labelLevel")
	{
		return Conv::toString(scale.labelLevel.get());
	}
	else throw std::logic_error("invalid channel parameter: " + property);
}

std::list<std::string> Config::listChannelParams()
{
	return {
		"title", 
		"set", 
		"stackable", 
		"range.min", 
		"range.max", 
		"labelLevel",
		"axis",
		"ticks",
		"interlacing",
		"guides"
	};
}

Config::Accessors Config::initAccessors()
{
	Accessors res;

	res.insert({ "title", {
		.get = [](const Options &options) {
			return Conv::toString(options.title.get());
		},
		.set = [](OptionsSetter &setter, const std::string &value)
		{
			setter.setTitle(
				Conv::parse<std::optional<std::string>>(value));
		}
	}});

	res.insert({ "legend", {
		.get = [](const Options &options) {
			return Conv::toString(options.legend.get());
		},
		.set = [](OptionsSetter &setter, const std::string &value)
		{
			Options::Legend legend(value);
			setter.setLegend(legend);
		}
	}});

	res.insert({ "coordSystem", {
		.get = [](const Options &options) {
			typedef CoordSystem::EnumType CS;
			CoordSystem cs { options.polar.get() ? CS::polar : CS::cartesian };
			return Conv::toString(cs);
		},
		.set = [](OptionsSetter &setter, const std::string &value)
		{
			typedef CoordSystem::EnumType CS;
			CS coordSys = CoordSystem(value);
			setter.setPolar(coordSys == CS::polar);
		} 
	}});

	res.insert({ "rotate", {
		.get = [](const Options &options) {
			return Conv::toString(90 * options.angle.get() / (M_PI / 2));
		},
		.set = [](OptionsSetter &setter, const std::string &value)
		{
			setter.rotate(Conv::parse<double>(value) / 90);
		}
	}});

	res.insert({ "geometry", {
		.get = [](const Options &options) {
			Geometry res((int)options.shapeType.get().type());
			return Conv::toString(res);
		},
		.set = [](OptionsSetter &setter, const std::string &value)
		{
			typedef Geometry::EnumType G;
			G geometry = Geometry(value);
			setter.setShape((ShapeType::Type)(int)(geometry));
		}
	}});

	res.insert({ "orientation", {
		.get = [](const Options &options) {
			typedef Orientation::EnumType O;
			Orientation res(options.horizontal.get() ? O::horizontal : O::vertical);
			return Conv::toString(res);
		},
		.set = [](OptionsSetter &setter, const std::string &value)
		{
			typedef Orientation::EnumType O;
			O orientation = Orientation(value);
			setter.setHorizontal(orientation == O::horizontal);
		}
	}});

	res.insert({ "sort", {
		.get = [](const Options &options) {
			typedef Sort::EnumType S;
			Sort res(options.sorted.get() ? S::byValue : S::none);
			return Conv::toString(res);
		},
		.set = [](OptionsSetter &setter, const std::string &value)
		{
			typedef Sort::EnumType S;
			S sort = Sort(value);
			setter.setSorted(sort == S::byValue);
		}
	}});

	res.insert({ "reverse", {
		.get = [](const Options &options) {
			return Conv::toString((bool)options.reverse.get());
		},
		.set = [](OptionsSetter &setter, const std::string &value)
		{
			setter.setReverse(Conv::parse<bool>(value));
		}
	}});

	res.insert({ "align", {
		.get = [](const Options &options) {
			Align res((int)options.alignType.get());
			return Conv::toString(res);
		},
		.set = [](OptionsSetter &setter, const std::string &value)
		{
			typedef Align::EnumType A;
			A align = Align(value);
			auto alignType = (Base::Align::Type)(int)align;
			setter.setAlign(alignType);
		}
	}});

	res.insert({ "split", {
		.get = [](const Options &options) {
			return Conv::toString((bool)options.splitted.get());
		},
		.set = [](OptionsSetter &setter, const std::string &value)
		{
			setter.setSplitted(Conv::parse<bool>(value));
		}
	}});

	res.insert({ "tooltip", {
		.get = [](const Options &options) {
			auto id = options.tooltipId.get();
			return id == Options::nullMarkerId
				? std::string("null") : Conv::toString(id);
		},
		.set = [](OptionsSetter &setter, const std::string &value)
		{
			if (value == "null")
				setter.showTooltip(Options::nullMarkerId);
			else
				setter.showTooltip(Conv::parse<int>(value));
		}
	}});

	return res;
}
