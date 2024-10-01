#ifndef CHANNEL_H
#define CHANNEL_H

#include <cstdint>
#include <optional>
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

class Channel
{
public:
	using Type = ChannelId;

	using OptionalIndex = std::optional<Data::SeriesIndex>;
	using IndexSet = std::set<Data::SeriesIndex>;
	using DimensionIndices = Data::SeriesList;

	static Channel makeChannel(Type id);

	void addSeries(const Data::SeriesIndex &index);
	void removeSeries(const Data::SeriesIndex &index);
	[[nodiscard]] bool isSeriesUsed(
	    const Data::SeriesIndex &index) const;
	void reset();
	[[nodiscard]] bool isEmpty() const;
	[[nodiscard]] bool isDimension() const;
	[[nodiscard]] bool hasDimension() const;
	[[nodiscard]] bool isMeasure() const;
	void collectDimesions(IndexSet &dimensions) const;
	[[nodiscard]] const DimensionIndices &dimensions() const;
	[[nodiscard]] std::pair<const DimensionIndices &,
	    const std::size_t &>
	dimensionsWithLevel() const;
	[[nodiscard]] OptionalIndex labelSeries() const;
	bool operator==(const Channel &other) const;

	Type type{};
	double defaultValue{};
	bool stackable{};
	OptionalIndex measureId{};
	DimensionIndices dimensionIds{};
	ChannelRange range{};
	std::size_t labelLevel{};
	Base::AutoParam<std::string, true> title{};
	Base::AutoBool axisLine{};
	Base::AutoBool axisLabels{};
	Base::AutoBool ticks{};
	Base::AutoBool guides{};
	Base::AutoBool markerGuides{};
	Base::AutoBool interlacing{};
	Base::AutoParam<double> step{};
};

std::optional<AxisId> asAxis(ChannelId type);
ChannelId asChannel(AxisId type);

}

#endif
