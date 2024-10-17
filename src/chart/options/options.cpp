
#include "options.h"

#include <array>
#include <atomic>
#include <cstddef>
#include <optional>

#include "base/math/trig.h"
#include "dataframe/old/types.h"

#include "align.h"
#include "channel.h"
#include "channelrange.h"
#include "coordsystem.h"
#include "orientation.h"
#include "shapetype.h"

namespace Vizzu::Gen
{

namespace
{
ChannelExtrema operator"" _perc(long double percent)
{
	return {static_cast<double>(percent),
	    ChannelExtremaType::relative};
}
}

[[nodiscard]] bool operator==(const Options::LegendId &l,
    const ChannelId &c)
{
	return Options::toChannel(l) == c;
}

void Options::reset()
{
	channels.reset();
	title = title.get().has_value() ? Heading(std::string())
	                                : Heading(std::nullopt);
	subtitle = subtitle.get().has_value() ? Heading(std::string())
	                                      : Heading(std::nullopt);
	caption = caption.get().has_value() ? Heading(std::string())
	                                    : Heading(std::nullopt);
}

const Channel *Options::subAxisOf(ChannelId id) const
{
	switch (geometry.get()) {
	case ShapeType::rectangle:
		return id == mainAxisType() ? &subAxis() : nullptr;

	case ShapeType::area:
		if (id == mainAxisType()) return &subAxis();
		if (id == subAxisType()) return &mainAxis();
		return nullptr;
	case ShapeType::line:
		return id == subAxisType()
		            || (id == ChannelId::size
		                && channels.anyAxisSet())
		         ? &channels.at(ChannelId::size)
		         : nullptr;

	case ShapeType::circle:
		if (id == ChannelId::size && channels.anyAxisSet()) {
			return &channels.at(ChannelId::size);
		}
		if (asAxis(id)) {
			if (channels.at(id).isDimension() && id == mainAxisType())
				return &subAxis();
			return &channels.at(ChannelId::size);
		}
		[[fallthrough]];
	default: return nullptr;
	}
}

ChannelId Options::stackChannelType() const
{
	if (channels.anyAxisSet()) {
		switch (geometry.get()) {
		case ShapeType::area:
		case ShapeType::rectangle: return asChannel(subAxisType());
		default:
		case ShapeType::circle:
		case ShapeType::line: return ChannelId::size;
		}
	}
	return ChannelId::size;
}

std::optional<ChannelId> Options::secondaryStackType() const
{
	if (channels.anyAxisSet() && geometry == ShapeType::line)
		return asChannel(subAxisType());

	return std::nullopt;
}

bool Options::isStacked() const
{
	auto dims = stackChannel().dimensions();
	dims.split_by(mainAxis().dimensions());
	return !dims.empty();
}

Channels Options::shadowChannels() const
{
	auto shadow = channels.shadow();

	auto &&stackChannel = stackChannelType();
	auto &&secondary = secondaryStackType();
	auto &&stackChannels = {stackChannel,
	    secondary.value_or(ChannelId{})};

	for (auto &ch1 = shadow.at(stackChannel),
	          &ch2 = shadow.at(ChannelId::noop);
	     auto &&stacker : shadow.getDimensions({data(stackChannels),
	         std::size_t{1} + secondary.has_value()})) {
		ch1.removeSeries(stacker);
		ch2.removeSeries(stacker);
	}

	return shadow;
}

void Options::drilldownTo(const Options &other)
{
	auto &stackChannel = this->stackChannel();

	if (this->split && !isSplit()) this->split = false;

	for (auto &&dim : other.getChannels().getDimensions())
		if (!getChannels().isSeriesUsed(dim))
			stackChannel.addSeries(dim);
}

void Options::intersection(const Options &other)
{
	for (auto &&dim : getChannels().getDimensions())
		if (!other.getChannels().isSeriesUsed(dim))
			getChannels().removeSeries(dim);
}

bool Options::looksTheSame(const Options &other) const
{
	if (channels.anyAxisSet()
	    && channels.at(Gen::ChannelId::label).isEmpty()) {
		auto thisCopy = *this;
		thisCopy.simplify();

		auto otherCopy = other;
		otherCopy.simplify();

		return thisCopy == otherCopy;
	}

	return *this == other;
}

void Options::simplify()
{
	//	remove all dimensions, only used at the end of stack
	auto &stackChannel = this->stackChannel();

	auto dimensions = stackChannel.dimensions();

	auto copy = getChannels();
	copy.at(stackChannelType()).reset();

	for (auto dim = dimensions.rbegin();
	     dim != Channel::DimensionIndices::rend()
	     && !copy.isSeriesUsed(*dim) && !labelsShownFor(*dim);
	     ++dim) {
		stackChannel.removeSeries(*dim);
	}
}

bool Options::operator==(const Options &other) const
{
	return channels == other.channels && sameAttributes(other);
}

bool Options::sameShadow(const Options &other) const
{
	return shadowChannels() == other.shadowChannels()
	    && sameShadowAttribs(other);
}

bool Options::sameShadowAttribs(const Options &other) const
{
	auto shape = geometry;
	if (shape == ShapeType::line) shape = ShapeType::area;

	auto shapeOther = other.geometry;
	if (shapeOther == ShapeType::line) shapeOther = ShapeType::area;

	return shape == shapeOther && coordSystem == other.coordSystem
	    && angle == other.angle && orientation == other.orientation
	    && isSplit() == other.isSplit()
	    && dataFilter == other.dataFilter && align == other.align
	    && sort == other.sort && reverse == other.reverse;
}

bool Options::sameAttributes(const Options &other) const
{
	return sameShadowAttribs(other) && geometry == other.geometry
	    && title == other.title && subtitle == other.subtitle
	    && caption == other.caption && legend == other.legend
	    && markersInfo == other.markersInfo;
}

AxisId Options::getHorizontalChannel() const
{
	return Math::rad2quadrant(angle) % 2 == 0 ? AxisId::x : AxisId::y;
}

AxisId Options::getVerticalChannel() const
{
	return getHorizontalChannel() == AxisId::x ? AxisId::y
	                                           : AxisId::x;
}

bool Options::isShapeValid(const ShapeType &shapeType) const
{
	if (mainAxis().hasDimension()) return true;
	return shapeType == ShapeType::rectangle
	    || shapeType == ShapeType::circle;
}

std::optional<Options::MarkerInfoId> Options::getMarkerInfoId(
    const MarkerIndex &id) const
{
	for (auto &&[gid, mkid] : markersInfo)
		if (mkid == id) return gid;
	return {};
}

Options::MarkerInfoId Options::generateMarkerInfoId()
{
	static std::atomic<MarkerInfoId> nextMarkerInfoId{1};
	return nextMarkerInfoId.fetch_add(1, std::memory_order_acq_rel);
}

void Options::setAutoParameters()
{
	if (auto &leg = *legend; leg.value.isAuto()) {
		leg.weight = 1.0;
		leg.value.setAuto(getAutoLegend());
	}

	if (auto &ori = *orientation; ori.value.isAuto()) {
		ori.weight = 1.0;
		ori.value.setAuto(getAutoOrientation());
	}
}

Orientation Options::getAutoOrientation() const
{
	if (const auto &x = getChannels().at(AxisId::x),
	    &y = getChannels().at(AxisId::y);
	    x.isMeasure()
	    && (y.isDimension()
	        || (!x.hasDimension() && y.hasDimension())))
		return Gen::Orientation::vertical;

	return Gen::Orientation::horizontal;
}

std::optional<Options::LegendId> Options::getAutoLegend() const
{
	auto series = channels.getDimensions();
	series.merge(channels.getMeasures());

	for (const auto &id : channels.at(ChannelId::label).dimensions())
		series.erase(id);

	if (auto &&meas = channels.at(ChannelId::label).measureId)
		series.erase(*meas);

	for (auto axisId : {AxisId::x, AxisId::y})
		if (auto id = channels.at(axisId).labelSeries())
			series.erase(*id);

	for (auto channelId : {LegendId::color, LegendId::lightness})
		if (channels.at(toChannel(channelId))
		        .dimensions()
		        .contains_any(series.begin(), series.end()))
			return channelId;

	for (auto channelId :
	    {LegendId::color, LegendId::lightness, LegendId::size})
		if (auto &&mid = channels.at(toChannel(channelId)).measureId)
			if (series.contains(*mid)) return channelId;

	return std::nullopt;
}

void Options::setAutoRange(bool hPositive, bool vPositive)
{
	auto &v = getVeritalAxis();
	auto &h = getHorizontalAxis();
	auto &&cart = coordSystem.get() == CoordSystem::cartesian;
	auto &&nrect = geometry != ShapeType::rectangle;

	if (cart && h.isMeasure() && (v.isDimension() || nrect))
		setMeasureRange(h, hPositive);
	else if (!cart && h.isMeasure() && v.isDimension()
	         && v.hasDimension())
		setRange(h, 0.0_perc, 133.0_perc);
	else
		setRange(h, 0.0_perc, 100.0_perc);

	if (v.isMeasure() && (h.isDimension() || (cart && nrect)))
		setMeasureRange(v, vPositive);
	else
		setRange(v, 0.0_perc, 100.0_perc);
}

void Options::setMeasureRange(Channel &channel, bool positive)
{
	if (positive)
		setRange(channel, 0.0_perc, 110.0_perc);
	else
		setRange(channel,
		    ChannelExtrema(-10, ChannelExtremaType::relative),
		    110.0_perc);
}

void Options::setRange(Channel &channel,
    ChannelExtrema min,
    ChannelExtrema max)
{
	if (channel.range.max.isAuto()) channel.range.max.setAuto(max);

	if (channel.range.min.isAuto()) channel.range.min.setAuto(min);
}

bool Options::labelsShownFor(const Data::SeriesIndex &series) const
{
	return channels.at(AxisId::x).labelSeries() == series
	    || channels.at(AxisId::y).labelSeries() == series
	    || (legend.get()
	        && channels.at(toChannel(*legend.get())).labelSeries()
	               == series);
}

void Options::showTooltip(std::optional<MarkerIndex> marker)
{
	if (!marker && tooltip) {
		if (auto &&miid = getMarkerInfoId(*tooltip))
			markersInfo.erase(*miid);
		tooltip.reset();
	}
	else if (marker && !tooltip) {
		if (!getMarkerInfoId(*marker))
			markersInfo.insert({generateMarkerInfoId(), *marker});
		tooltip = marker;
	}
	else if (marker && tooltip && marker != tooltip) {
		if (auto idFrom = getMarkerInfoId(*tooltip);
		    idFrom && !getMarkerInfoId(*marker))
			markersInfo.find(*idFrom)->second = *marker;
		tooltip = marker;
	}
}

[[nodiscard]] ChannelId Options::toChannel(const LegendId &l)
{
	return static_cast<ChannelId>(l);
}

}