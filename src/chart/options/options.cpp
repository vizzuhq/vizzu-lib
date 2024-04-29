
#include "options.h"

#include <atomic>

#include "base/math/trig.h"

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
		return id == mainAxisType() ? &subAxis()
		     : id == subAxisType()  ? &mainAxis()
		                            : nullptr;

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
		if (isAxis(id)) {
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
		case ShapeType::rectangle: return subAxisType();
		default:
		case ShapeType::circle:
		case ShapeType::line: return ChannelId::size;
		}
	}
	else
		return ChannelId::size;
}

std::optional<ChannelId> Options::secondaryStackType() const
{
	if (channels.anyAxisSet() && geometry == ShapeType::line)
		return subAxisType();

	return std::nullopt;
}

Channels Options::shadowChannels() const
{
	auto shadow = channels.shadow();

	std::vector<ChannelId> stackChannels;
	stackChannels.push_back(stackChannelType());
	if (auto &&secondary = secondaryStackType())
		stackChannels.push_back(*secondary);

	for (auto &&stacker : shadow.getDimensions(stackChannels)) {
		shadow.removeSeries(stackChannelType(), stacker);
		shadow.removeSeries(ChannelId::noop, stacker);
	}

	return shadow;
}

void Options::drilldownTo(const Options &other)
{
	auto &stackChannel = this->stackChannel();

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

	auto dimensions = stackChannel.dimensionIds;

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
	    && split == other.split && dataFilter == other.dataFilter
	    && align == other.align && sort == other.sort
	    && reverse == other.reverse;
}

bool Options::sameAttributes(const Options &other) const
{
	return sameShadowAttribs(other) && geometry == other.geometry
	    && title == other.title && subtitle == other.subtitle
	    && caption == other.caption && legend == other.legend
	    && markersInfo == other.markersInfo;
}

ChannelId Options::getHorizontalChannel() const
{
	return Math::rad2quadrant(angle) % 2 == 0 ? ChannelId::x
	                                          : ChannelId::y;
}

ChannelId Options::getVerticalChannel() const
{
	return getHorizontalChannel() == ChannelId::x ? ChannelId::y
	                                              : ChannelId::x;
}

bool Options::isShapeValid(const ShapeType &shapeType) const
{
	if (channels.anyAxisSet() && mainAxis().dimensionCount() > 0)
		return true;
	return shapeType == ShapeType::rectangle
	    || shapeType == ShapeType::circle;
}

std::optional<uint64_t> Options::getMarkerInfoId(MarkerId id) const
{
	for (auto &&[gid, mkid] : markersInfo)
		if (mkid == id) return gid;
	return {};
}

uint64_t Options::generateMarkerInfoId()
{
	static std::atomic_uint64_t nextMarkerInfoId = 1;
	return nextMarkerInfoId++;
}

void Options::setAutoParameters()
{
	if (legend.get().isAuto()) {
		LegendType tmp = legend.get();
		tmp.setAuto(getAutoLegend());
		legend = tmp;
	}
	if (orientation.get().isAuto()) {
		auto tmp = orientation.get();
		tmp.setAuto(getAutoOrientation());
		orientation = tmp;
	}
}

Orientation Options::getAutoOrientation() const
{
	if (getChannels().anyAxisSet()) {
		const auto &x = getChannels().at(ChannelId::x);
		const auto &y = getChannels().at(ChannelId::y);

		if (x.isEmpty() && !y.isDimension())
			return Gen::Orientation::horizontal;
		if (y.isEmpty() && !x.isDimension())
			return Gen::Orientation::vertical;

		if (!x.dimensionIds.empty() && y.dimensionIds.empty()
		    && !y.isDimension())
			return Gen::Orientation::horizontal;
		if (!y.dimensionIds.empty() && x.dimensionIds.empty()
		    && !x.isDimension())
			return Gen::Orientation::vertical;

		if (!x.dimensionIds.empty() && !y.dimensionIds.empty()) {
			if (x.isDimension() && !y.isDimension())
				return Gen::Orientation::horizontal;
			if (y.isDimension() && !x.isDimension())
				return Gen::Orientation::vertical;
		}
	}
	return Gen::Orientation::horizontal;
}

std::optional<Options::LegendId> Options::getAutoLegend() const
{
	auto series = channels.getDimensions();
	series.merge(channels.getMeasures());

	for (const auto &id : channels.at(ChannelId::label).dimensionIds)
		series.erase(id);

	if (channels.at(ChannelId::label).measureId)
		series.erase(*channels.at(ChannelId::label).measureId);

	for (auto channelId : {ChannelId::x, ChannelId::y})
		if (auto id = channels.at(channelId).labelSeries())
			series.erase(*id);

	for (auto channelId : {LegendId::color, LegendId::lightness})
		for (const auto &id :
		    channels.at(toChannel(channelId)).dimensionIds)
			if (series.contains(id)) return channelId;

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

	if (!channels.anyAxisSet()) {
		setRange(h, 0.0_perc, 100.0_perc);
		setRange(v, 0.0_perc, 100.0_perc);
	}
	else if (coordSystem.get() != CoordSystem::polar) {
		if (!h.isDimension() && !v.isDimension()
		    && geometry == ShapeType::rectangle) {
			setRange(h, 0.0_perc, 100.0_perc);
			setRange(v, 0.0_perc, 100.0_perc);
		}
		else {
			if (h.isDimension())
				setRange(h, 0.0_perc, 100.0_perc);
			else
				setMeasureRange(h, hPositive);

			if (v.isDimension())
				setRange(v, 0.0_perc, 100.0_perc);
			else
				setMeasureRange(v, vPositive);
		}
	}
	else {
		if (!h.isDimension() && v.isDimension()) {
			if (v.isEmpty()) {
				setRange(h, 0.0_perc, 100.0_perc);
				setRange(v, 0.0_perc, 100.0_perc);
			}
			else {
				setRange(h, 0.0_perc, 133.0_perc);
				setRange(v, 0.0_perc, 100.0_perc);
			}
		}
		else if (h.isDimension() && !v.isDimension()) {
			setRange(h, 0.0_perc, 100.0_perc);
			setMeasureRange(v, vPositive);
		}
		else {
			setRange(h, 0.0_perc, 100.0_perc);
			setRange(v, 0.0_perc, 100.0_perc);
		}
	}
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
	return channels.at(ChannelId::x).labelSeries() == series
	    || channels.at(ChannelId::y).labelSeries() == series
	    || (legend.get()
	        && channels.at(toChannel(*legend.get())).labelSeries()
	               == series);
}

void Options::showTooltip(std::optional<MarkerId> marker)
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