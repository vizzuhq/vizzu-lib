#include "config.h"

#include <functional>
#include <optional>
#include <stdexcept>
#include <string>

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

	for (auto &&channelParams = getAccessorNames<Channel>();
	     auto channelName : Refl::enum_names<ChannelId>) {
		for (const auto &param : channelParams)
			arr << "channels." + std::string{channelName} + "."
			           + std::string{param};

		if (channelName == "x" || channelName == "y")
			for (const auto &param :
			    getAccessorNames<AxisChannelProperties>())
				arr << "channels." + std::string{channelName} + "."
				           + std::string{param};
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
	auto channelId = Conv::parse<ChannelId>(parts.at(1));
	auto &channel = options.getChannels().at(channelId);
	auto property = parts.at(2);

	if (property == "attach" || property == "detach") {
		if (parts.size() == 3) return;
		property = "set";
	}

	if (property == "set") {
		if (parts.size() == 3) {
			channel.set = {};
			options.markersInfo.clear();
			return;
		}

		auto &listParser = ChannelSeriesList::Parser::instance();
		if (parts.size() == 4) {
			if (parts[3] == "begin") {
				if (parts[2] == "set") channel.set = {};
				options.markersInfo.clear();
				listParser.table = &table.get();
				listParser.channels.resize(std::stoull(value));
				return;
			}
			listParser.current = std::nullopt;
		}
		else
			listParser.current = std::stoull(parts.at(3));
		listParser.path = parts;
	}

	if (property == "range") property += "." + parts.at(3);

	if (auto &&accessor = getAccessor<Channel>(property).set)
		accessor(channel, value);
	else if (auto &&axisAccessor =
	             getAccessor<AxisChannelProperties>(property).set;
	         (channelId == AxisId::x || channelId == AxisId::y)
	         && axisAccessor)
		axisAccessor(
		    options.getChannels().axisPropsAt(
		        channelId == AxisId::x ? AxisId::x : AxisId::y),
		    value);
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

	if (property == "range") property += "." + parts.at(3);

	if (auto &&accessor = getAccessor<Channel>(property).get)
		return accessor(channel);
	if (auto &&axisAccessor =
	        getAccessor<AxisChannelProperties>(property).get;
	    (id == AxisId::x || id == AxisId::y) && axisAccessor)
		return axisAccessor(options.get().getChannels().axisPropsAt(
		    id == AxisId::x ? AxisId::x : AxisId::y));

	throw std::logic_error(path + ": invalid channel parameter");
}
}