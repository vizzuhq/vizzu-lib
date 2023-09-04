#include "config.h"

#include "base/conv/auto_json.h"
#include "base/conv/parse.h"
#include "base/refl/auto_struct.h"
#include "base/text/smartstring.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

const Config::Accessors &Config::getAccessors()
{
	static auto accessors = Config::initAccessors();
	return accessors;
}

template <class T> struct ExtractIf : std::identity
{
	using type = T;
};

template <class T> struct ExtractIf<::Anim::Interpolated<T>>
{
	using type = T;
	constexpr const T &operator()(
	    const ::Anim::Interpolated<T> &value) const
	{
		return value.get();
	}
};

template <> struct ExtractIf<Math::FuzzyBool>
{
	using type = bool;

	constexpr bool operator()(const Math::FuzzyBool &value) const
	{
		return static_cast<bool>(value);
	}
};

template <auto Mptr,
    auto Set,
    class T = ExtractIf<std::remove_cvref_t<
        std::invoke_result_t<decltype(Mptr), Options>>>>
inline constexpr std::pair<std::string_view, Config::Accessor>
    Config::accessor = {
        Refl::Variables::MemberName<
            Refl::Members::MemberCast<Mptr>::getName()>,
        {.get =
                [](const Options &options)
            {
	            return Conv::toString(
	                T{}(std::invoke(Mptr, options)));
            },
            .set =
                [](OptionsSetter &setter, const std::string &value)
            {
	            std::invoke(Set,
	                setter,
	                Conv::parse<typename T::type>(value));
            }}};

std::list<std::string> Config::listParams()
{
	std::list<std::string> res;
	for (const auto &accessor : getAccessors())
		res.emplace_back(accessor.first);

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

	if (property == "title") { setter.setAxisTitle(id, value); }
	else if (property == "axis") {
		setter.setAxisLine(id, Conv::parse<Base::AutoBool>(value));
	}
	else if (property == "labels") {
		setter.setAxisLabels(id, Conv::parse<Base::AutoBool>(value));
	}
	else if (property == "ticks") {
		setter.setTicks(id, Conv::parse<Base::AutoBool>(value));
	}
	else if (property == "interlacing") {
		setter.setInterlacing(id, Conv::parse<Base::AutoBool>(value));
	}
	else if (property == "guides") {
		setter.setGuides(id, Conv::parse<Base::AutoBool>(value));
	}
	else if (property == "markerGuides") {
		setter.setMarkerGuides(id,
		    Conv::parse<Base::AutoBool>(value));
	}
	else if (property == "step") {
		setter.setStep(id,
		    Conv::parse<Base::AutoParam<double>>(value));
	}
	else if (property == "attach") {
		setter.addSeries(id, value);
	}
	else if (property == "detach") {
		setter.deleteSeries(id, value);
	}
	else if (property == "set") {
		if (parts.size() == 3 && value == "null")
			setter.clearSeries(id);
		else {
			if (std::stoi(parts.at(3)) == 0) setter.clearSeries(id);
			setter.addSeries(id, value);
		}
	}
	else if (property == "stackable") {
		setter.setStackable(id, Conv::parse<bool>(value));
	}
	else if (property == "range") {
		if (parts.size() >= 4 && parts.at(3) == "min") {
			setter.setRangeMin(id,
			    Conv::parse<OptionalChannelExtrema>(value));
		}
		else if (parts.size() >= 4 && parts.at(3) == "max") {
			setter.setRangeMax(id,
			    Conv::parse<OptionalChannelExtrema>(value));
		}
		else
			throw std::logic_error(
			    path + "/" + value + ": invalid range setting");
	}
	else if (property == "labelLevel") {
		setter.setLabelLevel(id, Conv::parse<int>(value));
	}
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

	const auto &channel = setter.getOptions().getChannels().at(id);

	if (property == "title") { return Conv::toString(channel.title); }
	if (property == "axis") {
		return Conv::toString(channel.axisLine);
	}
	if (property == "labels") {
		return Conv::toString(channel.axisLabels);
	}
	if (property == "ticks") { return Conv::toString(channel.ticks); }
	if (property == "interlacing") {
		return Conv::toString(channel.interlacing);
	}
	if (property == "guides") {
		return Conv::toString(channel.guides);
	}
	if (property == "markerGuides") {
		return Conv::toString(channel.markerGuides);
	}
	if (property == "set") {
		auto list = channel.dimensionNames(*setter.getTable());
		auto measure = channel.measureName(*setter.getTable());
		if (!measure.empty()) list.push_front(measure);
		return Conv::toJSON(list);
	}
	if (property == "stackable") {
		return Conv::toString(channel.stackable);
	}
	if (property == "range") {
		if (parts.size() == 4 && parts.at(3) == "min") {
			return Conv::toString(channel.range.min);
		}
		if (parts.size() == 4 && parts.at(3) == "max") {
			return Conv::toString(channel.range.max);
		}
		throw std::logic_error(path + ": invalid range parameter");
	}
	if (property == "labelLevel") {
		return Conv::toString(channel.labelLevel);
	}
	throw std::logic_error(path + ": invalid channel parameter");
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

	res.emplace(accessor<&Options::title, &OptionsSetter::setTitle>);
	res.emplace(
	    accessor<&Options::legend, &OptionsSetter::setLegend>);
	res.emplace(accessor<&Options::coordSystem,
	    &OptionsSetter::setCoordSystem>);

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

	res.emplace(
	    accessor<&Options::geometry, &OptionsSetter::setShape>);
	res.emplace(accessor<&Options::orientation,
	    &OptionsSetter::setOrientation>);
	res.emplace(accessor<&Options::sort, &OptionsSetter::setSorted>);
	res.emplace(
	    accessor<&Options::reverse, &OptionsSetter::setReverse>);
	res.emplace(accessor<&Options::align, &OptionsSetter::setAlign>);
	res.emplace(
	    accessor<&Options::split, &OptionsSetter::setSplitted>);
	res.emplace(
	    accessor<&Options::tooltip, &OptionsSetter::showTooltip>);

	return res;
}
