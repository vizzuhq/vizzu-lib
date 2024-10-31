#ifndef CHANNEL_H
#define CHANNEL_H

#include <cstdint>
#include <optional>
#include <set>
#include <string>

#include "dataframe/old/types.h"

#include "autoparam.h"
#include "channelrange.h"

namespace Vizzu::Gen
{

enum class ChannelId : std::uint8_t {
	color,
	lightness,
	size,
	label,
	x,
	y,
	noop
};
using ChannelIdType = std::underlying_type_t<ChannelId>;

enum class AxisId : ChannelIdType {
	x = static_cast<ChannelIdType>(ChannelId::x),
	y = static_cast<ChannelIdType>(ChannelId::y)
};

consteval auto unique_from_to(AxisId)
{
	return std::pair{static_cast<ChannelIdType>(AxisId::x),
	    static_cast<ChannelIdType>(AxisId::y)};
}

static_assert(Refl::enum_names<AxisId>.size() == 2);
static_assert(std::ranges::all_of(Refl::enum_names<AxisId>,
    [](std::string_view name)
    {
	    return static_cast<ChannelIdType>(
	               Refl::get_enum<AxisId>(name))
	        == static_cast<ChannelIdType>(
	            Refl::get_enum<ChannelId>(name));
    }));

enum class LegendId : ChannelIdType {
	color = static_cast<ChannelIdType>(ChannelId::color),
	lightness = static_cast<ChannelIdType>(ChannelId::lightness),
	size = static_cast<ChannelIdType>(ChannelId::size)
};

static_assert(Refl::enum_names<LegendId>.size() == 3);
static_assert(std::ranges::all_of(Refl::enum_names<LegendId>,
    [](std::string_view name)
    {
	    return static_cast<ChannelIdType>(
	               Refl::get_enum<LegendId>(name))
	        == static_cast<ChannelIdType>(
	            Refl::get_enum<ChannelId>(name));
    }));

struct ChannelSeriesList
{
	using OptionalIndex = std::optional<Data::SeriesIndex>;
	using DimensionIndices = Data::SeriesList;
	OptionalIndex measureId{};
	DimensionIndices dimensionIds{};

	[[nodiscard]] std::string toString() const;

	struct FromString
	{
		enum class Parse : std::uint8_t { null, name, aggregator };
		const Data::DataTable *table{};
		ChannelId latestChannel{};
		std::size_t position{};
		Parse type{};
		std::optional<Data::SeriesIndex> res{};

		[[nodiscard]] FromString &operator()(const std::string &str);
	} static &fromString;

	ChannelSeriesList &operator=(FromString &);

	bool operator==(const ChannelSeriesList &) const = default;
};

class Channel
{
public:
	using OptionalIndex = ChannelSeriesList::OptionalIndex;
	using IndexSet = std::set<Data::SeriesIndex>;
	using DimensionIndices = ChannelSeriesList::DimensionIndices;

	[[nodiscard]] static Channel makeChannel(ChannelId id);

	void addSeries(const Data::SeriesIndex &index);
	void removeSeries(const Data::SeriesIndex &index);
	[[nodiscard]] bool isSeriesUsed(
	    const Data::SeriesIndex &index) const;
	void reset();
	[[nodiscard]] bool isEmpty() const;
	[[nodiscard]] bool isDimension() const;
	[[nodiscard]] bool hasDimension() const;
	[[nodiscard]] bool isMeasure() const;
	void collectDimensions(IndexSet &dimensions) const;
	[[nodiscard]] const DimensionIndices &dimensions() const;
	[[nodiscard]] std::pair<const DimensionIndices &,
	    const std::size_t &>
	dimensionsWithLevel() const;
	[[nodiscard]] OptionalIndex labelSeries() const;
	[[nodiscard]] bool operator==(
	    const Channel &other) const = default;

	[[nodiscard]] const OptionalIndex &measure() const
	{
		return set.measureId;
	}

	bool stackable{};
	Base::AutoParam<std::string, true> title{};
	ChannelRange range{};
	std::size_t labelLevel{};
	Base::AutoBool axis{};
	Base::AutoBool ticks{};
	Base::AutoBool interlacing{};
	Base::AutoBool guides{};
	Base::AutoBool markerGuides{};
	Base::AutoBool labels{};
	Base::AutoParam<double> step{};
	ChannelSeriesList set{};
};

[[nodiscard]] constexpr ChannelId asChannel(AxisId axis)
{
	return static_cast<ChannelId>(axis);
}

[[nodiscard]] constexpr ChannelId asChannel(const LegendId &legend)
{
	return static_cast<ChannelId>(legend);
}

template <class T>
concept ChannelIdLike = requires(const T &v) {
	{
		asChannel(v)
	} -> std::same_as<ChannelId>;
};

template <ChannelIdLike T>
[[nodiscard]] constexpr bool operator==(const T &l,
    const ChannelId &c)
{
	return asChannel(l) == c;
}

template <ChannelIdLike T, ChannelIdLike U>
    requires(!std::same_as<T, U>)
[[nodiscard]] constexpr bool operator==(const T &, const U &)
{
	return false;
}

}

#endif
