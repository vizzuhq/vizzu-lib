#include "config.h"

#include "base/conv/auto_json.h"
#include "base/conv/parse.h"
#include "base/refl/auto_struct.h"
#include "base/text/smartstring.h"

namespace Vizzu::Gen
{

template <class T> struct ExtractIf : std::identity
{
	using type = T;
};

template <class T> struct ExtractIf<::Anim::Interpolated<T>>
{
	using type = T;
	const T &operator()(const ::Anim::Interpolated<T> &value) const
	{
		return value.get();
	}
};

template <> struct ExtractIf<Math::FuzzyBool>
{
	using type = bool;

	bool operator()(const Math::FuzzyBool &value) const
	{
		return static_cast<bool>(value);
	}
};

template <auto Mptr>
using ExtractType =
    ExtractIf<std::remove_cvref_t<std::invoke_result_t<decltype(Mptr),
        decltype(Refl::Name::getBase(Mptr))>>>;

template <auto Mptr>
inline constexpr std::pair<std::string_view, Config::Accessor>
    Config::accessor = {Refl::Name::in_data_name<
                            Refl::Name::name<decltype(Mptr), Mptr>()>,
        {.get =
                [](const Options &options)
            {
	            return Conv::toString(
	                ExtractType<Mptr>{}(std::invoke(Mptr, options)));
            },
            .set =
                [](Options &options, const std::string &value)
            {
	            std::invoke(Mptr, options) =
	                Conv::parse<typename ExtractType<Mptr>::type>(
	                    value);
            }}};

const Config::Accessors &Config::getAccessors()
{
	static const Config::Accessors accessors = {
	    accessor<&Options::title>,
	    accessor<&Options::subtitle>,
	    accessor<&Options::caption>,
	    accessor<&Options::legend>,
	    accessor<&Options::coordSystem>,
	    accessor<&Options::angle>,
	    accessor<&Options::geometry>,
	    accessor<&Options::orientation>,
	    accessor<&Options::sort>,
	    accessor<&Options::reverse>,
	    accessor<&Options::align>,
	    accessor<&Options::split>,
	    {"tooltip",
	        {.get =
	                [](const Options &options)
	            {
		            return Conv::toString(options.tooltip);
	            },
	            .set =
	                [](Options &options, const std::string &value)
	            {
		            options.showTooltip(Conv::parse<
		                std::optional<Options::MarkerIndex>>(value));
	            }}}};

	return accessors;
}

template <auto Mptr>
const std::pair<std::string_view, Config::ChannelAccessor>
    Config::channel_accessor{
        Refl::Name::in_data_name<
            Refl::Name::name<decltype(Mptr), Mptr>()>,
        {.get =
                [](const Channel &channel)
            {
	            return Conv::toString(
	                ExtractType<Mptr>{}(std::invoke(Mptr, channel)));
            },
            .set =
                [](Channel &channel, const std::string &value)
            {
	            std::invoke(Mptr, channel) =
	                Conv::parse<typename ExtractType<Mptr>::type>(
	                    value);
            }}};

const Config::ChannelAccessors &Config::getChannelAccessors()
{
	static const ChannelAccessors accessors{
	    channel_accessor<&Channel::title>,
	    {"set", {}},
	    channel_accessor<&Channel::stackable>,
	    {"range.min",
	        {.get =
	                [](const Channel &channel)
	            {
		            return Conv::toString(channel.range.min);
	            },
	            .set =
	                [](Channel &channel, const std::string &value)
	            {
		            channel.range.min =
		                Conv::parse<OptionalChannelExtrema>(value);
	            }}},
	    {"range.max",
	        {.get =
	                [](const Channel &channel)
	            {
		            return Conv::toString(channel.range.max);
	            },
	            .set =
	                [](Channel &channel, const std::string &value)
	            {
		            channel.range.max =
		                Conv::parse<OptionalChannelExtrema>(value);
	            }}},
	    channel_accessor<&Channel::labelLevel>,
	    {"axis", channel_accessor<&Channel::axisLine>.second},
	    channel_accessor<&Channel::ticks>,
	    channel_accessor<&Channel::interlacing>,
	    channel_accessor<&Channel::guides>,
	    channel_accessor<&Channel::markerGuides>,
	    {"labels", channel_accessor<&Channel::axisLabels>.second},
	    channel_accessor<&Channel::step>};
	return accessors;
}

std::string Config::paramsJson()
{
	std::string res;
	Conv::JSONArr arr{res};
	for (const auto &accessor : getAccessors()) arr << accessor.first;

	for (auto &&channelParams = getChannelAccessors();
	     auto channelName : Refl::enum_names<ChannelId>)
		for (const auto &param : channelParams)
			arr << "channels." + std::string{channelName} + "."
			           + std::string{param.first};
	return res;
}

void Config::setParam(const std::string &path,
    const std::string &value)
{
	if (path.starts_with("channels.")) {
		setChannelParam(path, value);
	}
	else {
		auto it = getAccessors().find(path);
		if (it == getAccessors().end())
			throw std::logic_error(
			    path + "/" + value + ": invalid config parameter");
		it->second.set(options, value);
	}
}

std::string Config::getParam(const std::string &path) const
{
	if (path.starts_with("channels.")) return getChannelParam(path);

	if (auto it = getAccessors().find(path);
	    it != getAccessors().end())
		return it->second.get(options);

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

	const auto &accessors = getChannelAccessors();
	if (auto it = accessors.find(property); it != accessors.end())
		return it->second.set(channel, value);

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

	const auto &accessors = getChannelAccessors();
	if (auto it = accessors.find(property); it != accessors.end())
		return it->second.get(channel);

	throw std::logic_error(path + ": invalid channel parameter");
}
}