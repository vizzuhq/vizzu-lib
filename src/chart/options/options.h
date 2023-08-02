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
#include "data/datacube/datafilter.h"
#include "data/multidim/multidimindex.h"
#include "data/table/datatable.h"

#include "align.h"
#include "autoparam.h"
#include "channels.h"
#include "shapetype.h"

namespace Vizzu::Gen
{

class Options
{
public:
	using MarkerId = uint64_t;
	using Title = ::Anim::Interpolated<std::optional<std::string>>;
	using Legend = ::Anim::Interpolated<Base::AutoParam<ChannelId>>;
	using MarkersInfoMap = std::map<uint64_t, MarkerId>;

	Options();

	[[nodiscard]] const Channels &getChannels() const
	{
		return channels;
	}
	Channels &getChannels() { return channels; }

	void reset();

	[[nodiscard]] ChannelId mainAxisType() const
	{
		return horizontal ? ChannelId::x : ChannelId::y;
	}

	[[nodiscard]] ChannelId subAxisType() const
	{
		return horizontal ? ChannelId::y : ChannelId::x;
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
	[[nodiscard]] ChannelId stackAxisType() const;
	[[nodiscard]] std::optional<ChannelId> secondaryStackType() const;

	Channel &stackAxis() { return channels.at(stackAxisType()); }

	Title title;
	Math::FuzzyBool polar;
	double angle;
	Anim::Interpolated<ShapeType> shapeType;
	Math::FuzzyBool horizontal;
	Math::FuzzyBool splitted;
	Base::Align::Type alignType{Base::Align::Type::none};
	Data::Filter dataFilter;
	Math::FuzzyBool sorted;
	Math::FuzzyBool reverse;
	Legend legend;
	std::optional<uint64_t> tooltipId;
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

	[[nodiscard]] ChannelId getHorizontalChannel() const;
	[[nodiscard]] ChannelId getVerticalChannel() const;

	[[nodiscard]] const Channel &getHorizontalAxis() const
	{
		return channels.at(getHorizontalChannel());
	}

	[[nodiscard]] const Channel &getVeritalAxis() const
	{
		return channels.at(getVerticalChannel());
	}

	Channel &getHorizontalAxis()
	{
		return channels.at(getHorizontalChannel());
	}

	Channel &getVeritalAxis()
	{
		return channels.at(getVerticalChannel());
	}

	[[nodiscard]] bool isShapeValid(const ShapeType &) const;
	[[nodiscard]] std::optional<uint64_t> getMarkerInfoId(
	    MarkerId) const;
	static uint64_t generateMarkerInfoId();

	void setAutoParameters();
	void setAutoRange(bool hPositive, bool vPositive);

private:
	Channels channels;

	std::optional<ChannelId> getAutoLegend();
	static void setMeasureRange(Channel &channel, bool positive);
	static void setRange(Channel &channel,
	    ChannelExtrema min,
	    ChannelExtrema max);
	static uint64_t nextMarkerInfoId;
};

using PlotOptionsPtr = std::shared_ptr<Options>;

}

#endif
