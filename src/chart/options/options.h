#ifndef PLOT_OPTIONS_H
#define PLOT_OPTIONS_H

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

#include "base/anim/interpolated.h"
#include "base/geom/orientation.h"
#include "base/geom/rect.h"
#include "base/math/fuzzybool.h"
#include "base/math/range.h"
#include "dataframe/old/types.h"

#include "autoparam.h"
#include "channels.h"
#include "coordsystem.h"
#include "shapetype.h"

namespace Vizzu::Gen
{

struct OptionProperties
{
	using Heading = ::Anim::Interpolated<std::optional<std::string>>;
	using LegendType = Base::AutoParam<LegendId, true>;
	using Legend = ::Anim::Interpolated<LegendType>;
	using OrientationType = Base::AutoParam<Geom::Orientation>;
	using Orientation = ::Anim::Interpolated<OrientationType>;

	Heading title{std::nullopt};
	Heading subtitle{std::nullopt};
	Heading caption{std::nullopt};
	Legend legend;
	::Anim::Interpolated<CoordSystem> coordSystem{
	    CoordSystem::cartesian};
	double angle{};
	::Anim::Interpolated<ShapeType> geometry{ShapeType::rectangle};
	Orientation orientation{OrientationType{}};
};

class Options : public OptionProperties
{
public:
	using MarkerIndex = std::string;
	using MarkerInfoId = std::uint32_t;
	using Heading = ::Anim::Interpolated<std::optional<std::string>>;
	using MarkersInfoMap = std::map<MarkerInfoId, MarkerIndex>;

	[[nodiscard]] const Channels &getChannels() const
	{
		return channels;
	}
	Channels &getChannels() { return channels; }

	void reset();

	[[nodiscard]] AxisId mainAxisType() const
	{
		return isHorizontal() ? AxisId::x : AxisId::y;
	}

	[[nodiscard]] bool isHorizontal() const
	{
		return Geom::isHorizontal(getOrientation());
	}

	[[nodiscard]] Geom::Orientation getOrientation() const
	{
		auto hasOrientation = orientation.get();
		return hasOrientation ? *hasOrientation
		                      : getAutoOrientation();
	}

	[[nodiscard]] AxisId subAxisType() const
	{
		return !mainAxisType();
	}

	[[nodiscard]] const Channel &mainAxis() const
	{
		return channels.at(mainAxisType());
	}

	[[nodiscard]] const Channel &subAxis() const
	{
		return channels.at(subAxisType());
	}

	[[nodiscard]] std::vector<std::size_t> dimLabelIndex(
	    ChannelId channel) const
	{
		auto &&ch = channels.at(channel);
		auto hasMeasure = ch.hasMeasure();

		auto defaultLabelDimension =
		    hasMeasure && channel == mainAxisType()
		    && ch.hasDimension() && geometry == ShapeType::rectangle;
		auto lll = ch.labelLevel.getValueOrAuto();
		if (!lll && defaultLabelDimension) {
			lll.emplace(std::size_t{1});
		}
		else if (!lll && !hasMeasure && ch.dimensions().size() > 1
		         && channel == legend.get().getValueOrAuto()) {
			lll.emplace(
			    Channel::LabelLevelList{std::vector{std::from_range,
			        std::views::iota(std::size_t{},
			            ch.dimensions().size())}});
		}
		else if (!lll) {
			lll.emplace(std::size_t{0});
		}

		if (hasMeasure && std::ranges::contains(lll->levels, 0))
			return {};
		if (hasMeasure)
			std::ranges::transform(lll->levels,
			    lll->levels.begin(),
			    [](auto &&v)
			    {
				    return v - 1;
			    });
		std::erase_if(lll->levels,
		    [max = ch.dimensions().size()](auto &v)
		    {
			    return v >= max;
		    });
		return lll->levels;
	}

	[[nodiscard]] bool isMeasure(ChannelId channel) const
	{
		return channels.at(channel).hasMeasure()
		    && dimLabelIndex(channel).empty();
	}

	template <ChannelIdLike IdType>
	[[nodiscard]] std::vector<Data::SeriesIndex> labelSeries(
	    IdType channel) const
	{
		auto &&ch = channels.at(channel);
		if (auto dimIndex = dimLabelIndex(+channel);
		    !dimIndex.empty()) {
			std::vector<Data::SeriesIndex> result(dimIndex.size());
			std::transform(dimIndex.begin(),
			    dimIndex.end(),
			    result.begin(),
			    [&ch](std::size_t index)
			    {
				    return *std::next(ch.set.dimensionIds.begin(),
				        static_cast<std::intptr_t>(index));
			    });
			return result;
		}
		if (auto &&measure = ch.measure();
		    measure
		    && ch.labelLevel.getValue(std::size_t{}).levels
		           == std::vector{std::size_t{}})
			return {*measure};
		return {};
	}

	Channel &mainAxis() { return channels.at(mainAxisType()); }

	Channel &subAxis() { return channels.at(subAxisType()); }

	[[nodiscard]] const Channel *subAxisOf(ChannelId id) const;
	[[nodiscard]] ChannelId stackChannelType() const;
	[[nodiscard]] std::optional<ChannelId> secondaryStackType() const;

	Channel &stackChannel()
	{
		return channels.at(stackChannelType());
	}

	[[nodiscard]] const Channel &stackChannel() const
	{
		return channels.at(stackChannelType());
	}

	[[nodiscard]] bool hasDimensionToSplit(AxisId at) const;
	[[nodiscard]] bool isSplit(AxisId byAxis) const
	{
		return getChannels().axisPropsAt(byAxis).split
		    && hasDimensionToSplit(byAxis);
	}
	Data::Filter dataFilter;
	std::optional<MarkerIndex> tooltip;
	MarkersInfoMap markersInfo;

	bool operator==(const Options &other) const;
	[[nodiscard]] bool sameShadow(const Options &other) const;
	[[nodiscard]] bool looksTheSame(const Options &other) const;
	[[nodiscard]] bool sameAttributes(const Options &other) const;
	[[nodiscard]] bool sameShadowAttribs(const Options &other) const;
	[[nodiscard]] Channels shadowChannels() const;
	void drilldownTo(const Options &other);
	void intersection(const Options &other);
	void simplify();

	[[nodiscard]] AxisId getHorizontalChannel() const;

	[[nodiscard]] bool isShapeValid(const ShapeType &) const;
	[[nodiscard]] std::optional<MarkerInfoId> getMarkerInfoId(
	    const MarkerIndex &) const;
	static MarkerInfoId generateMarkerInfoId();

	void setAutoParameters();
	void setAutoRange(bool hPositive,
	    bool vPositive,
	    bool isSplit = false);

	[[nodiscard]] bool labelsShownFor(
	    const Data::SeriesIndex &series) const;

	void showTooltip(std::optional<MarkerIndex> marker);

private:
	Channels channels{
	    []<std::size_t... Ix>(std::index_sequence<Ix...>)
	    {
		    return decltype(channels){
		        {{Channel::makeChannel(
		            static_cast<ChannelId>(Ix))...}},
		        {}};
	    }(std::make_index_sequence<std::size(
	            Refl::enum_values<ChannelId>())>())};

	[[nodiscard]] Geom::Orientation getAutoOrientation() const;
	[[nodiscard]] std::optional<LegendId> getAutoLegend() const;
	static void setMeasureRange(Channel &channel, bool positive);
	static void setRange(Channel &channel,
	    ChannelExtrema min,
	    ChannelExtrema max);
};

using PlotOptionsPtr = std::shared_ptr<Options>;

}

#endif
