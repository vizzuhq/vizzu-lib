
#include "options.h"

#include <atomic>
#include <cstddef>
#include <iterator>
#include <optional>

#include "base/geom/orientation.h"
#include "base/math/trig.h"
#include "dataframe/old/types.h"

#include "channel.h"
#include "channelrange.h"
#include "coordsystem.h"
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
		if (id == AxisId::x || id == AxisId::y) {
			if (!isMeasure(id) && id == mainAxisType())
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
		case ShapeType::rectangle: return +subAxisType();
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
		return +subAxisType();

	return std::nullopt;
}

bool Options::hasDimensionToSplit() const
{
	auto dims = subAxis().dimensions();
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
		if (stackChannelType() != subAxisType() || !isSplit())
			ch1.removeSeries(stacker);
		ch2.removeSeries(stacker);
	}

	return shadow;
}

void Options::drilldownTo(const Options &other)
{
	auto &stackChannel = this->stackChannel();

	if (!isSplit() || !other.isSplit()) this->split = {};

	for (auto &&dim : other.getChannels().getDimensions())
		if (!getChannels().isSeriesUsed(dim))
			stackChannel.addSeries(dim);
}

void Options::intersection(const Options &other)
{
	for (auto &&dim : getChannels().getDimensions())
		if (!other.getChannels().isSeriesUsed(dim))
			getChannels().removeSeries(dim);

	split = {};
}

bool Options::looksTheSame(const Options &other) const
{
	if (channels.anyAxisSet()
	    && channels.at(ChannelId::label).isEmpty()) {
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
	if (isSplit()) return;

	//	remove all dimensions, only used at the end of stack
	auto &stackChannel = this->stackChannel();

	if (auto &&meas = stackChannel.measure())
		if (meas->getAggr() != dataframe::aggregator_type::sum)
			return;

	if (auto &&meas = this->mainAxis().measure())
		if (meas->getAggr() != dataframe::aggregator_type::sum)
			return;

	if (auto &&leg = this->legend.get())
		if (auto &&meas = this->channels.at(*leg).measure())
			if (meas->getAggr() != dataframe::aggregator_type::sum)
				return;

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
	if (auto &ori = *orientation; ori.value.isAuto()) {
		ori.weight = 1.0;
		ori.value.setAuto(getAutoOrientation());
	}

	if (auto &leg = *legend; leg.value.isAuto()) {
		leg.weight = 1.0;
		leg.value.setAuto(getAutoLegend());
	}
}

Geom::Orientation Options::getAutoOrientation() const
{
	if (const auto &x = getChannels().at(AxisId::x),
	    &y = getChannels().at(AxisId::y);
	    x.hasMeasure()
	    && (!y.hasMeasure()
	        || (!x.hasDimension() && y.hasDimension())))
		return Geom::Orientation::vertical;

	return Geom::Orientation::horizontal;
}

std::optional<LegendId> Options::getAutoLegend() const
{
	auto series = channels.getDimensions();
	series.merge(channels.getMeasures());

	for (const auto &id : channels.at(ChannelId::label).dimensions())
		series.erase(id);

	if (auto &&meas = channels.at(ChannelId::label).measure())
		series.erase(*meas);

	for (auto axisId : {AxisId::x, AxisId::y})
		if (auto &&id = labelSeries(axisId)) series.erase(*id);

	for (auto channelId : {LegendId::color, LegendId::lightness})
		if (channels.at(channelId).dimensions().contains_any(
		        series.begin(),
		        series.end()))
			return channelId;

	for (auto channelId :
	    {LegendId::color, LegendId::lightness, LegendId::size})
		if (auto &&mid = channels.at(channelId).measure())
			if (series.contains(*mid)) return channelId;

	return std::nullopt;
}

void Options::setAutoRange(bool hPositive, bool vPositive)
{
	auto &v = getVerticalAxis();
	auto &h = getHorizontalAxis();
	auto vHasMeasure = getVerticalAxis().hasMeasure();
	auto hHasMeasure = getHorizontalAxis().hasMeasure();
	auto &&cart = coordSystem.get() == CoordSystem::cartesian;
	auto &&nrect = geometry != ShapeType::rectangle;

	if (cart && hHasMeasure && (!vHasMeasure || nrect))
		setMeasureRange(h, hPositive);
	else if (!cart && hHasMeasure && !vHasMeasure && v.hasDimension())
		setRange(h, 0.0_perc, 133.0_perc);
	else
		setRange(h, 0.0_perc, 100.0_perc);

	if (vHasMeasure && (!hHasMeasure || (cart && nrect)))
		setMeasureRange(v, vPositive);
	else
		setRange(v, 0.0_perc, 100.0_perc);
}

void Options::setMeasureRange(Channel &channel, bool positive)
{
	if (positive)
		setRange(channel, 0.0_perc, 110.0_perc);
	else
		setRange(channel, operator""_perc(-10.0), 110.0_perc);
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
	return labelSeries(AxisId::x) == series
	    || labelSeries(AxisId::y) == series
	    || (legend.get() && labelSeries(*legend.get()) == series);
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

}