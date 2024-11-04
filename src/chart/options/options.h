#ifndef PLOT_OPTIONS_H
#define PLOT_OPTIONS_H

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "base/anim/interpolated.h"
#include "base/geom/rect.h"
#include "base/math/fuzzybool.h"
#include "base/math/range.h"
#include "dataframe/old/types.h"

#include "align.h"
#include "autoparam.h"
#include "channels.h"
#include "coordsystem.h"
#include "orientation.h"
#include "shapetype.h"
#include "sort.h"

namespace Vizzu::Gen
{

struct OptionProperties
{
	using Heading = ::Anim::Interpolated<std::optional<std::string>>;
	using LegendType = Base::AutoParam<LegendId, true>;
	using Legend = ::Anim::Interpolated<LegendType>;
	using OrientationType = Base::AutoParam<Gen::Orientation>;
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
	Sort sort{Sort::none};
	Math::FuzzyBool reverse{false};
	Base::Align::Type align{Base::Align::Type::none};
	Math::FuzzyBool split;
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
		auto hasOrientation = orientation.get();
		return (hasOrientation ? *hasOrientation
		                       : getAutoOrientation())
		    == Gen::Orientation::horizontal;
	}

	[[nodiscard]] AxisId subAxisType() const
	{
		return isHorizontal() ? AxisId::y : AxisId::x;
	}

	[[nodiscard]] const Channel &mainAxis() const
	{
		return channels.at(mainAxisType());
	}

	[[nodiscard]] const Channel &subAxis() const
	{
		return channels.at(subAxisType());
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

	[[nodiscard]] bool isStacked() const;
	[[nodiscard]] bool isSplit() const
	{
		return split
		    && (subAxisType() != stackChannelType() || isStacked());
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
	[[nodiscard]] AxisId getVerticalChannel() const;

	[[nodiscard]] const Channel &getHorizontalAxis() const
	{
		return channels.at(getHorizontalChannel());
	}

	[[nodiscard]] const Channel &getVerticalAxis() const
	{
		return channels.at(getVerticalChannel());
	}

	Channel &getHorizontalAxis()
	{
		return channels.at(getHorizontalChannel());
	}

	Channel &getVerticalAxis()
	{
		return channels.at(getVerticalChannel());
	}

	[[nodiscard]] bool isShapeValid(const ShapeType &) const;
	[[nodiscard]] std::optional<MarkerInfoId> getMarkerInfoId(
	    const MarkerIndex &) const;
	static MarkerInfoId generateMarkerInfoId();

	void setAutoParameters();
	void setAutoRange(bool hPositive, bool vPositive);

	[[nodiscard]] bool labelsShownFor(
	    const Data::SeriesIndex &series) const;

	void showTooltip(std::optional<MarkerIndex> marker);

private:
	Channels channels{
	    []<std::size_t... Ix>(std::index_sequence<Ix...>)
	    {
		    return decltype(channels){
		        Channel::makeChannel(static_cast<ChannelId>(Ix))...};
	    }(std::make_index_sequence<
	        std::tuple_size_v<decltype(channels)::base_array>>{})};

	[[nodiscard]] Gen::Orientation getAutoOrientation() const;
	[[nodiscard]] std::optional<LegendId> getAutoLegend() const;
	static void setMeasureRange(Channel &channel, bool positive);
	static void setRange(Channel &channel,
	    ChannelExtrema min,
	    ChannelExtrema max);
};

using PlotOptionsPtr = std::shared_ptr<Options>;

}

#endif
