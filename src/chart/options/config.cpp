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
		auto &listParser = ChannelSeriesList::Parser::instance();
		listParser.table = &table.get();
		using Token = ChannelSeriesList::Parser::Token;
		if ((parts.size() == 3 && value == "null")
		    || (parts.size() == 5 && parts[3] == "0"
		        && parts[4] == "name")) {

			std::optional<dataframe::aggregator_type> aggregator;
			if (auto &&res = listParser.res) {
				if (res->isDimension())
					throw std::runtime_error(
					    "Multiple dimension at channel "
					    + std::string{Conv::toString(
					        listParser.latestChannel)}
					    + ": " + res->getColIndex());

				if (listParser.type == Token::aggregator) {
					if (parts.size() == 5 && listParser.position == 0
					    && res->getColIndex().empty()
					    && listParser.latestChannel == channelId)
						aggregator = listParser.res->getAggr();
					else
						throw std::runtime_error(
						    "Aggregator has no set name at channel "
						    + std::string{Conv::toString(
						        listParser.latestChannel)}
						    + ": "
						    + std::string{
						        Conv::toString(res->getAggr())});
				}
			}

			channel.reset();
			options.markersInfo.clear();

			listParser.type = Token::null;
			listParser.res = {};

			if (aggregator)
				listParser.res.emplace().setAggr(aggregator);
		}
		listParser.latestChannel = channelId;
		if (parts.size() == 5) {
			if (auto i = std::stoull(parts.at(3));
			    i != listParser.position) {
				if (auto &&res = listParser.res) {
					if (res->isDimension())
						throw std::runtime_error(
						    "Multiple dimension at channel "
						    + parts.at(1) + ": "
						    + res->getColIndex());

					if (listParser.type == Token::aggregator)
						throw std::runtime_error(
						    "Aggregator has no set name at channel "
						    + parts.at(1) + ": "
						    + std::string{
						        Conv::toString(res->getAggr())});

					res.reset();
				}
				listParser.position = i;
			}
			if (parts.at(4) == "name")
				listParser.type = Token::name;
			else if (parts.at(4) == "aggregator")
				listParser.type = Token::aggregator;
			else
				throw std::logic_error(
				    path + ": invalid channel parameter");
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