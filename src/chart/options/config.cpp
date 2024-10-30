#include "config.h"

#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

#include "base/conv/auto_json.h"
#include "base/conv/parse.h"
#include "base/conv/tostring.h"
#include "base/refl/auto_accessor.h"
#include "base/refl/auto_enum.h"
#include "base/text/smartstring.h"
#include "chart/options/channel.h"
#include "chart/options/options.h"

namespace Vizzu::Gen
{

using Refl::Access::getAccessor;
using Refl::Access::getAccessorNames;

std::string Config::paramsJson()
{
	std::string res;
	Conv::JSONArr arr{res};
	for (const auto &accessor : getAccessorNames<OptionProperties>())
		arr << accessor;
	arr << "tooltip";

	for (auto &&channelParams = getAccessorNames<ChannelProperties>();
	     auto channelName : Refl::enum_names<ChannelId>) {
		for (const auto &param : channelParams)
			arr << "channels." + std::string{channelName} + "."
			           + std::string{param};
		arr << "channels." + std::string{channelName} + ".set";
	}
	return res;
}

void Config::setParam(const std::string &path,
    const std::string &value)
{
	if (path.starts_with("channels."))
		setChannelParam(path, value);
	else if (path == "tooltip")
		options.get().showTooltip(
		    Conv::parse<std::optional<Options::MarkerIndex>>(value));
	else if (auto &&accessor =
	             getAccessor<OptionProperties>(path).set)
		accessor(options, value);
	else
		throw std::logic_error(
		    path + "/" + value + ": invalid config parameter");
}

std::string Config::getParam(const std::string &path) const
{
	if (path.starts_with("channels.")) return getChannelParam(path);

	if (path == "tooltip")
		return Conv::toString(options.get().tooltip);

	if (auto &&accessor = getAccessor<OptionProperties>(path).get)
		return accessor(options);

	throw std::logic_error(path + ": invalid config parameter");
}

void Config::setChannelParam(const std::string &path,
    const std::string &value)
{
	Options &options = this->options;

	auto parts = Text::SmartString::split(path, '.');
	auto &channel =
	    options.getChannels().at(Conv::parse<ChannelId>(parts.at(1)));
	auto property = parts.at(2);

	if (property == "attach") {
		options.markersInfo.clear();
		channel.addSeries({value, table});
		return;
	}
	if (property == "detach") {
		options.markersInfo.clear();
		channel.removeSeries({value, table});
		return;
	}
	if (property == "set") {
		options.markersInfo.clear();
		if (parts.size() == 3 && value == "null")
			channel.reset();
		else {
			if (const std::string_view command =
			        parts.size() == 4 ? std::string_view{"name"}
			                          : parts.at(4);
			    command == "name") {
				if (std::stoi(parts.at(3)) == 0) channel.reset();
				if (value.empty()) { channel.measureId.emplace(); }
				else
					channel.addSeries({value, table});
			}
			else if (command == "aggregator") {
				if (value != "null") {
					if (!channel.measureId.has_value())
						channel.measureId.emplace(
						    channel.dimensionIds.pop_back());

					channel.measureId->setAggr(value);
				}
				else if (channel.measureId)
					channel.measureId->setAggr(
					    channel.measureId->getColIndex().empty()
					        ? "count"
					        : "sum");
			}
		}
		return;
	}

	if (property == "range") property += "." + parts.at(3);

	if (auto &&accessor =
	        getAccessor<ChannelProperties>(property).set)
		accessor(channel, value);
	else
		throw std::logic_error(
		    path + "/" + value
		    + ": invalid channel parameter: " + property);
}

std::string Config::getChannelParam(const std::string &path) const
{
	auto parts = Text::SmartString::split(path, '.');
	auto id = Conv::parse<ChannelId>(parts.at(1));
	auto property = parts.at(2);

	const auto &channel = options.get().getChannels().at(id);

	if (property == "set") {
		std::string res;
		Conv::JSONArr arr{res};
		if (auto &&measure = channel.measureId) arr << *measure;
		for (auto &&dim : channel.dimensions()) arr << dim;
		return res;
	}

	if (property == "range") property += "." + parts.at(3);

	if (auto &&accessor =
	        getAccessor<ChannelProperties>(property).get)
		return accessor(channel);

	throw std::logic_error(path + ": invalid channel parameter");
}
}