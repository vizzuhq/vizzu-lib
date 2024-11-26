#ifndef CHANNEL_H
#define CHANNEL_H

#include <cstdint>
#include <optional>
#include <set>
#include <string>

#include "base/geom/orientation.h"
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

[[nodiscard]] constexpr Geom::Orientation operator++(
    const AxisId &axis)
{
	return axis == AxisId::x ? Geom::Orientation::horizontal
	                         : Geom::Orientation::vertical;
}

[[nodiscard]] constexpr AxisId operator!(const AxisId &axis)
{
	return axis == AxisId::x ? AxisId::y : AxisId::x;
}

struct ChannelSeriesList
{
	using OptionalIndex = std::optional<Data::SeriesIndex>;
	using DimensionIndices = Data::SeriesList;
	OptionalIndex measureId{};
	DimensionIndices dimensionIds{};

	[[nodiscard]] std::string toString() const;

	struct RawChannelSeries
	{
		std::optional<std::string> name;
		std::string aggregator;
	};

	struct Parser
	{
		const Data::DataTable *table{};
		std::vector<RawChannelSeries> channels;
		std::optional<std::size_t> current;
		std::span<std::string> path;

		[[nodiscard]] Parser &operator()(const std::string &str);

		static Parser &instance() noexcept;
	};

	struct ParserGetter
	{
		Parser &operator()(const std::string &str) const
		{
			return Parser::instance()(str);
		}
	} constexpr static fromString{};

	ChannelSeriesList &operator=(Parser &);

	bool operator==(const ChannelSeriesList &) const = default;

	bool addSeries(const Data::SeriesIndex &index);
	bool removeSeries(const Data::SeriesIndex &index);
};

struct AxisChannelProperties
{
	Base::AutoBool axis{};
	Base::AutoBool labels{};
	Base::AutoBool ticks{};
	Base::AutoBool interlacing{};
	Base::AutoBool guides{};
	Base::AutoBool markerGuides{};
	Base::AutoParam<double> step{};

	[[nodiscard]] bool operator==(
	    const AxisChannelProperties &) const = default;
};

struct Channel
{
	using OptionalIndex = ChannelSeriesList::OptionalIndex;
	using IndexSet = std::set<Data::SeriesIndex>;
	using DimensionIndices = ChannelSeriesList::DimensionIndices;

	void addSeries(const Data::SeriesIndex &index)
	{
		set.addSeries(index);
	}

	void removeSeries(const Data::SeriesIndex &index)
	{
		set.removeSeries(index);
	}

	[[nodiscard]] static Channel makeChannel(ChannelId id);
	[[nodiscard]] bool isSeriesUsed(
	    const Data::SeriesIndex &index) const;
	void reset();
	[[nodiscard]] bool isEmpty() const;
	[[nodiscard]] bool hasDimension() const;
	[[nodiscard]] bool hasMeasure() const;
	void collectDimensions(IndexSet &dimensions) const;
	[[nodiscard]] bool operator==(
	    const Channel &other) const = default;

	[[nodiscard]] const DimensionIndices &dimensions() const
	{
		return set.dimensionIds;
	}

	[[nodiscard]] const OptionalIndex &measure() const
	{
		return set.measureId;
	}

	bool stackable{};
	ChannelSeriesList set{};
	ChannelRange range{};
	Base::AutoParam<std::size_t> labelLevel{};
	Base::AutoParam<std::string, true> title{};
};

[[nodiscard]] constexpr ChannelId operator+(const AxisId &axis)
{
	return static_cast<ChannelId>(axis);
}

[[nodiscard]] constexpr ChannelId operator+(const LegendId &legend)
{
	return static_cast<ChannelId>(legend);
}

template <class T>
concept ChannelIdLike = requires(const T &v) {
	{
		+v
	} -> std::same_as<ChannelId>;
};

template <ChannelIdLike T>
[[nodiscard]] constexpr bool operator==(const T &l,
    const ChannelId &c)
{
	return +l == c;
}

template <ChannelIdLike T, ChannelIdLike U>
    requires(!std::same_as<T, U>)
[[nodiscard]] constexpr bool operator==(const T &, const U &)
{
	return false;
}

}

#endif
