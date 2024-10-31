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
#include "dataframe/interface.h"
#include "dataframe/old/datatable.h"

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
		auto &fromString = ChannelSeriesList::FromString::instance();
		fromString.table = &table.get();
		if ((parts.size() == 3 && value == "null")
		    || (parts.size() == 5 && parts[3] == "0"
		        && parts[4] == "name")) {
			auto needSaveAggregator =
			    fromString.position == 0
			    && fromString.type
			           == ChannelSeriesList::FromString::Parse::
			               aggregator
			    && fromString.res && !fromString.res->isDimension()
			    && fromString.res->getColIndex().empty()
			    && fromString.latestChannel == channelId;

			std::optional<dataframe::aggregator_type> aggregator;
			if (needSaveAggregator)
				aggregator = fromString.res->getAggr();

			channel.reset();
			options.markersInfo.clear();

			fromString.type =
			    ChannelSeriesList::FromString::Parse::null;
			fromString.res = {};

			if (aggregator)
				fromString.res.emplace().setAggr(aggregator);
		}
		fromString.latestChannel = channelId;
		if (parts.size() == 5) {
			fromString.type =
			    Conv::parse<ChannelSeriesList::FromString::Parse>(
			        parts[4]);
			if (auto i = std::stoull(parts.at(3));
			    i != fromString.position) {
				if (fromString.res) {
					if (fromString.res->isDimension())
						throw std::runtime_error(
						    "Multiple dimension at channel "
						    + parts.at(1) + ": "
						    + fromString.res->getColIndex());
					fromString.res = {};
				}
				fromString.position = i;
			}
		}
	}

	if (property == "range") property += "." + parts.at(3);

	if (auto &&accessor = getAccessor<Channel>(property).set)
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

	if (property == "range") property += "." + parts.at(3);

	if (auto &&accessor = getAccessor<Channel>(property).get)
		return accessor(channel);

	throw std::logic_error(path + ": invalid channel parameter");
}
}