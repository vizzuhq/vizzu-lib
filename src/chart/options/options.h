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

namespace Vizzu
{
namespace Gen
{

class Options
{
public:
	typedef uint64_t MarkerId;
	static constexpr MarkerId nullMarkerId = static_cast<uint64_t>(-1);
	static constexpr uint64_t nullMarkerInfoId = static_cast<uint64_t>(-1);
	typedef ::Anim::Interpolated<std::optional<std::string>> Title;
	typedef ::Anim::Interpolated<Base::AutoParam<ChannelId>> Legend;
	typedef std::map<uint64_t, MarkerId> MarkersInfoMap;

	Options();

	const Channels &getChannels() const { return channels; }
	Channels &getChannels() { return channels; }

	void reset();

	ChannelId mainAxisType() const
	{
		return horizontal ? ChannelId::x : ChannelId::y;
	}

	ChannelId subAxisType() const
	{
		return horizontal ? ChannelId::y : ChannelId::x;
	}

	const Channel &mainAxis() const
	{
		return channels.at(mainAxisType());
	}

	const Channel &subAxis() const { return channels.at(subAxisType()); }

	Channel &mainAxis() { return channels.at(mainAxisType()); }

	Channel &subAxis() { return channels.at(subAxisType()); }

	const Channel *subAxisOf(ChannelId id) const;
	ChannelId stackAxisType() const;
	std::optional<ChannelId> secondaryStackType() const;

	Channel &stackAxis() { return channels.at(stackAxisType()); }

	Title title;
	Math::FuzzyBool polar;
	double angle;
	Anim::Interpolated<ShapeType> shapeType;
	Math::FuzzyBool horizontal;
	Math::FuzzyBool splitted;
	Base::Align::Type alignType;
	Data::Filter dataFilter;
	Math::FuzzyBool sorted;
	Math::FuzzyBool reverse;
	Legend legend;
	uint64_t tooltipId;
	MarkersInfoMap markersInfo;

	bool operator==(const Options &other) const;
	bool sameShadow(const Options &other) const;
	bool looksTheSame(const Options &other) const;
	bool sameAttributes(const Options &other) const;
	bool sameShadowAttribs(const Options &other) const;
	Channels shadowChannels() const;
	void drilldownTo(const Options &other);
	void intersection(const Options &other);
	void simplify();

	ChannelId getHorizontalChannel() const;
	ChannelId getVerticalChannel() const;

	const Channel &getHorizontalAxis() const
	{
		return channels.at(getHorizontalChannel());
	}

	const Channel &getVeritalAxis() const
	{
		return channels.at(getVerticalChannel());
	}

	Channel &getHorizontalAxis()
	{
		return channels.at(getHorizontalChannel());
	}

	Channel &getVeritalAxis() { return channels.at(getVerticalChannel()); }

	bool isShapeValid(const ShapeType &) const;
	uint64_t getMarkerInfoId(MarkerId) const;
	uint64_t generateMarkerInfoId() const;

	void setAutoParameters();
	void setAutoRange(bool hPositive, bool vPositive);

private:
	Channels channels;

	std::optional<ChannelId> getAutoLegend();
	void setMeasureRange(Channel &channel, bool positive);
	void setRange(Channel &channel, ChannelExtrema min, ChannelExtrema max);
	static uint64_t nextMarkerInfoId;
};

typedef std::shared_ptr<Options> PlotOptionsPtr;

}
}

#endif
