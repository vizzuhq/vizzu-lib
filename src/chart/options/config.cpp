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

template <auto Mptr, auto Set>
inline constexpr std::pair<std::string_view, Config::Accessor>
    Config::unique_accessor = {
        Refl::Name::in_data_name<
            Refl::Name::name<decltype(Mptr), Mptr>()>,
        {.get =
                [](const Options &options)
            {
	            return Conv::toString(
	                ExtractType<Mptr>{}(std::invoke(Mptr, options)));
            },
            .set =
                [](OptionsSetter &setter, const std::string &value)
            {
	            std::invoke(Set,
	                setter,
	                Conv::parse<typename ExtractType<Mptr>::type>(
	                    value));
            }}};

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
                [](OptionsSetter &setter, const std::string &value)
            {
	            std::invoke(Mptr, setter.getOptions()) =
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
	    {"rotate",
	        {.get =
	                [](const Options &options)
	            {
		            return Conv::toString(
		                90 * options.angle / (M_PI / 2));
	            },
	            .set =
	                [](OptionsSetter &setter,
	                    const std::string &value)
	            {
		            setter.rotate(Conv::parse<double>(value) / 90);
	            }}},
	    accessor<&Options::geometry>,
	    accessor<&Options::orientation>,
	    accessor<&Options::sort>,
	    accessor<&Options::reverse>,
	    accessor<&Options::align>,
	    accessor<&Options::split>,
	    unique_accessor<&Options::tooltip,
	        &OptionsSetter::showTooltip>};

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
                [](OptionsSetter &setter,
                    const ChannelId &channel,
                    const std::string &value)
            {
	            std::invoke(Mptr,
	                setter.getOptions().getChannels().at(channel)) =
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
	                [](OptionsSetter &setter,
	                    const ChannelId &id,
	                    const std::string &value)
	            {
		            setter.getOptions()
		                .getChannels()
		                .at(id)
		                .range.min =
		                Conv::parse<OptionalChannelExtrema>(value);
	            }}},
	    {"range.max",
	        {.get =
	                [](const Channel &channel)
	            {
		            return Conv::toString(channel.range.max);
	            },
	            .set =
	                [](OptionsSetter &setter,
	                    const ChannelId &id,
	                    const std::string &value)
	            {
		            setter.getOptions()
		                .getChannels()
		                .at(id)
		                .range.max =
		                Conv::parse<OptionalChannelExtrema>(value);
	            }}},
	    channel_accessor<&Channel::labelLevel>,
	    {"axis", channel_accessor<&Channel::axisLine>.second},
	    channel_accessor<&Channel::ticks>,
	    channel_accessor<&Channel::interlacing>,
	    channel_accessor<&Channel::guides>,
	    channel_accessor<&Channel::markerGuides>,
	    {"labels", channel_accessor<&Channel::axisLabels>.second}};
	return accessors;
}

std::list<std::string> Config::listParams()
{
	std::list<std::string> res;
	for (const auto &accessor : getAccessors())
		res.emplace_back(accessor.first);

	auto channelParams =
	    std::ranges::views::keys(getChannelAccessors());
	for (auto channelName : Refl::enum_names<ChannelId>) {
		for (const auto &param : channelParams)
			res.push_back("channels." + std::string{channelName} + "."
			              + std::string{param});
	}

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
		it->second.set(setter, value);
	}
}

std::string Config::getParam(const std::string &path) const
{
	if (path.starts_with("channels.")) return getChannelParam(path);

	if (auto it = getAccessors().find(path);
	    it != getAccessors().end())
		return it->second.get(setter.getOptions());

	throw std::logic_error(path + ": invalid config parameter");
}

void Config::setFilter(Data::Filter::Function &&func, uint64_t hash)
{
	setter.setFilter(Data::Filter(std::move(func), hash));
}

void Config::setChannelParam(const std::string &path,
    const std::string &value)
{
	auto parts = Text::SmartString::split(path, '.');
	auto id = Conv::parse<ChannelId>(parts.at(1));
	auto property = parts.at(2);

	if (property == "step") {
		setter.setStep(id,
		    Conv::parse<Base::AutoParam<double>>(value));
		return;
	}
	if (property == "attach") {
		setter.addSeries(id, value);
		return;
	}
	if (property == "detach") {
		setter.deleteSeries(id, value);
		return;
	}
	if (property == "set") {
		if (parts.size() == 3 && value == "null")
			setter.clearSeries(id);
		else {
			if (std::stoi(parts.at(3)) == 0) setter.clearSeries(id);
			setter.addSeries(id, value);
		}
		return;
	}

	if (property == "range") property += "." + parts.at(3);

	const auto &accessors = getChannelAccessors();
	if (auto it = accessors.find(property); it != accessors.end()) {
		return it->second.set(setter, id, value);
	}

	throw std::logic_error(
	    path + "/" + value
	    + ": invalid channel parameter: " + property);
}

std::string Config::getChannelParam(const std::string &path) const
{
	auto parts = Text::SmartString::split(path, '.');
	auto id = Conv::parse<ChannelId>(parts.at(1));
	auto property = parts.at(2);

	const auto &channel = setter.getOptions().getChannels().at(id);

	if (property == "set") {
		auto list = channel.dimensionNames(*setter.getTable());
		auto measure = channel.measureName(*setter.getTable());
		if (!measure.empty()) list.push_front(measure);
		return Conv::toJSON(list);
	}

	if (property == "range") property += "." + parts.at(3);

	const auto &accessors = getChannelAccessors();
	if (auto it = accessors.find(property); it != accessors.end()) {
		return it->second.get(channel);
	}
	throw std::logic_error(path + ": invalid channel parameter");
}
}