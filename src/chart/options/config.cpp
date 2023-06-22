#include "config.h"

#include "base/conv/parse.h"
#include "base/io/log.h"
#include "base/text/jsonoutput.h"
#include "base/text/smartstring.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

const Config::Accessors Config::accessors = Config::initAccessors();

std::list<std::string> Config::listParams()
{
	std::list<std::string> res;
	for (const auto &accessor : accessors)
		res.push_back(accessor.first);

	auto channelParams = listChannelParams();
	for (auto channelName : Refl::enum_names<ChannelId>) {
		for (auto &param : channelParams)
			res.push_back(
			    "channels." + std::string{channelName} + "." + param);
	}

	return res;
}

void Config::setParam(const std::string &path,
    const std::string &value)
{
	if (Text::SmartString::startsWith(path, "channels.")) {
		setChannelParam(path, value);
	}
	else {
		auto it = accessors.find(path);
		if (it == accessors.end())
			throw std::logic_error(
			    "invalid config parameter: " + path);
		it->second.set(*setter, value);
	}
}

std::string Config::getParam(const std::string &path) const
{
	if (Text::SmartString::startsWith(path, "channels.")) {
		return getChannelParam(path);
	}
	else {
		auto it = accessors.find(path);
		if (it == accessors.end())
			throw std::logic_error(
			    "invalid config parameter: " + path);
		return it->second.get(setter->getOptions());
	}
}

void Config::setFilter(Data::Filter::Function &&func, uint64_t hash)
{
	setter->setFilter(Data::Filter(std::move(func), hash));
}

void Config::setChannelParam(const std::string &path,
    const std::string &value)
{
	auto parts = Text::SmartString::split(path, '.');
	auto id = Conv::parse<ChannelId>(parts.at(1));
	auto property = parts.at(2);

	if (property == "title") { setter->setTitle(id, value); }
	else if (property == "axis") {
		setter->setAxisLine(id, Conv::parse<Base::AutoBool>(value));
	}
	else if (property == "labels") {
		setter->setAxisLabels(id, Conv::parse<Base::AutoBool>(value));
	}
	else if (property == "ticks") {
		setter->setTicks(id, Conv::parse<Base::AutoBool>(value));
	}
	else if (property == "interlacing") {
		setter->setInterlacing(id,
		    Conv::parse<Base::AutoBool>(value));
	}
	else if (property == "guides") {
		setter->setGuides(id, Conv::parse<Base::AutoBool>(value));
	}
	else if (property == "markerGuides") {
		setter->setMarkerGuides(id,
		    Conv::parse<Base::AutoBool>(value));
	}
	else if (property == "step") {
		setter->setStep(id,
		    Conv::parse<Base::AutoParam<double>>(value));
	}
	else if (property == "attach") {
		setter->addSeries(id, value);
	}
	else if (property == "detach") {
		setter->deleteSeries(id, value);
	}
	else if (property == "set") {
		if (parts.size() == 3 && value == "null")
			setter->clearSeries(id);
		else {
			if (std::stoi(parts.at(3)) == 0) setter->clearSeries(id);
			setter->addSeries(id, value);
		}
	}
	else if (property == "stackable") {
		setter->setStackable(id, Conv::parse<bool>(value));
	}
	else if (property == "range") {
		if (parts.size() >= 4 && parts.at(3) == "min") {
			setter->setRangeMin(id,
			    Conv::parse<OptionalChannelExtrema>(value));
		}
		else if (parts.size() >= 4 && parts.at(3) == "max") {
			setter->setRangeMax(id,
			    Conv::parse<OptionalChannelExtrema>(value));
		}
		else
			throw std::logic_error("invalid range setting");
	}
	else if (property == "labelLevel") {
		setter->setLabelLevel(id, Conv::parse<uint64_t>(value));
	}
	else
		throw std::logic_error(
		    "invalid channel parameter: " + property);
}

std::string Config::getChannelParam(const std::string &path) const
{
	auto parts = Text::SmartString::split(path, '.');
	auto id = Conv::parse<ChannelId>(parts.at(1));
	auto property = parts.at(2);

	auto &channel = setter->getOptions().getChannels().at(id);

	if (property == "title") {
		return Conv::toString(channel.title);
	}
	else if (property == "axis") {
		return Conv::toString(channel.axisLine);
	}
	else if (property == "labels") {
		return Conv::toString(channel.axisLabels);
	}
	else if (property == "ticks") {
		return Conv::toString(channel.ticks);
	}
	else if (property == "interlacing") {
		return Conv::toString(channel.interlacing);
	}
	else if (property == "guides") {
		return Conv::toString(channel.guides);
	}
	else if (property == "markerGuides") {
		return Conv::toString(channel.markerGuides);
	}
	else if (property == "set") {
		auto list = channel.dimensionNames(*setter->getTable());
		auto measure = channel.measureName(*setter->getTable());
		if (!measure.empty()) list.push_front(measure);
		return Text::toJSon(list);
	}
	else if (property == "stackable") {
		return Conv::toString(channel.stackable);
	}
	else if (property == "range") {
		if (parts.size() == 4 && parts.at(3) == "min") {
			return Conv::toString(channel.range.min);
		}
		else if (parts.size() == 4 && parts.at(3) == "max") {
			return Conv::toString(channel.range.max);
		}
		else
			throw std::logic_error(
			    "invalid range parameter: " + path);
	}
	else if (property == "labelLevel") {
		return Conv::toString(channel.labelLevel);
	}
	else
		throw std::logic_error(
		    "invalid channel parameter: " + property);
}

std::list<std::string> Config::listChannelParams()
{
	return {"title",
	    "set",
	    "stackable",
	    "range.min",
	    "range.max",
	    "labelLevel",
	    "axis",
	    "ticks",
	    "interlacing",
	    "guides",
	    "markerGuides",
	    "labels"};
}

Config::Accessors Config::initAccessors()
{
	Accessors res;

	res.insert({"title",
	    {.get =
	            [](const Options &options)
	        {
		        return Conv::toString(options.title.get());
	        },
	        .set =
	            [](OptionsSetter &setter, const std::string &value)
	        {
		        setter.setTitle(
		            Conv::parse<std::optional<std::string>>(value));
	        }}});

	res.insert({"legend",
	    {.get =
	            [](const Options &options)
	        {
		        return Conv::toString(options.legend.get());
	        },
	        .set =
	            [](OptionsSetter &setter, const std::string &value)
	        {
		        Options::Legend legend(value);
		        setter.setLegend(legend);
	        }}});

	res.insert({"coordSystem",
	    {.get =
	            [](const Options &options)
	        {
		        auto cs{options.polar ? CoordSystem::polar
		                                    : CoordSystem::cartesian};
		        return Conv::toString(cs);
	        },
	        .set =
	            [](OptionsSetter &setter, const std::string &value)
	        {
		        auto coordSys = Conv::parse<CoordSystem>(value);
		        setter.setPolar(coordSys == CoordSystem::polar);
	        }}});

	res.insert({"rotate",
	    {.get =
	            [](const Options &options)
	        {
		        return Conv::toString(
		            90 * options.angle / (M_PI / 2));
	        },
	        .set =
	            [](OptionsSetter &setter, const std::string &value)
	        {
		        setter.rotate(Conv::parse<double>(value) / 90);
	        }}});

	res.insert({"geometry",
	    {.get =
	            [](const Options &options)
	        {
		        return Conv::toString(
		            options.shapeType);
	        },
	        .set =
	            [](OptionsSetter &setter, const std::string &value)
	        {
		        setter.setShape(Conv::parse<ShapeType>(value));
	        }}});

	res.insert({"orientation",
	    {.get =
	            [](const Options &options)
	        {
		        auto res(options.horizontal
		                     ? Orientation::horizontal
		                     : Orientation::vertical);
		        return Conv::toString(res);
	        },
	        .set =
	            [](OptionsSetter &setter, const std::string &value)
	        {
		        auto orientation = Conv::parse<Orientation>(value);
		        setter.setHorizontal(
		            orientation == Orientation::horizontal);
	        }}});

	res.insert({"sort",
	    {.get =
	            [](const Options &options)
	        {
		        auto res(options.sorted ? Sort::byValue
		                                      : Sort::none);
		        return Conv::toString(res);
	        },
	        .set =
	            [](OptionsSetter &setter, const std::string &value)
	        {
		        auto sort = Conv::parse<Sort>(value);
		        setter.setSorted(sort == Sort::byValue);
	        }}});

	res.insert({"reverse",
	    {.get =
	            [](const Options &options)
	        {
		        return Conv::toString((bool)options.reverse);
	        },
	        .set =
	            [](OptionsSetter &setter, const std::string &value)
	        {
		        setter.setReverse(Conv::parse<bool>(value));
	        }}});

	res.insert({"align",
	    {.get =
	            [](const Options &options)
	        {
		        return Conv::toString(options.alignType);
	        },
	        .set =
	            [](OptionsSetter &setter, const std::string &value)
	        {
		        setter.setAlign(Conv::parse<Base::Align::Type>(value));
	        }}});

	res.insert({"split",
	    {.get =
	            [](const Options &options)
	        {
		        return Conv::toString((bool)options.splitted);
	        },
	        .set =
	            [](OptionsSetter &setter, const std::string &value)
	        {
		        setter.setSplitted(Conv::parse<bool>(value));
	        }}});

	res.insert({"tooltip",
	    {.get =
	            [](const Options &options)
	        {
		        auto id = options.tooltipId;
		        return id == Options::nullMarkerId
		                 ? std::string("null")
		                 : Conv::toString(id);
	        },
	        .set =
	            [](OptionsSetter &setter, const std::string &value)
	        {
		        if (value == "null")
			        setter.showTooltip(Options::nullMarkerId);
		        else
			        setter.showTooltip(Conv::parse<int>(value));
	        }}});

	return res;
}
