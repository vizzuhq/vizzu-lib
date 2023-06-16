
#include "options.h"

#include "base/math/trig.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

static ChannelExtrema operator"" _perc(long double percent)
{
	return ChannelExtrema(percent, ChannelExtremaType::relative);
}

uint64_t Options::nextMarkerInfoId = 1;

Options::Options()
{
	alignType.set(Base::Align::None);
	polar.set(false);
	angle.set(0.0);
	horizontal.set(true);
	sorted.set(false);
	reverse.set(false);
	title.set(std::nullopt);
	tooltipId.set(nullMarkerId);
}

void Options::reset()
{
	channels.reset();
	title.set(title.get().get().has_value() ? Title(std::string())
	                                        : Title(std::nullopt));
}

const Channel *Options::subAxisOf(ChannelId id) const
{
	switch ((ShapeType::Type)shapeType.get()) {
	case ShapeType::Type::Rectangle:
		return id == mainAxisType() ? &subAxis() : nullptr;

	case ShapeType::Type::Area:
		return id == mainAxisType() ? &subAxis()
		     : id == subAxisType()  ? &mainAxis()
		                            : nullptr;

	case ShapeType::Type::Line:
		return id == subAxisType()
		            || (id == ChannelId::size && channels.anyAxisSet())
		         ? &channels.at(ChannelId::size)
		         : nullptr;

	case ShapeType::Type::Circle:
		if (id == ChannelId::size && channels.anyAxisSet()) {
			return &channels.at(ChannelId::size);
		}
		else if (isAxis(id)) {
			if (channels.at(id).isPseudoDimension()
			    && id == mainAxisType())
				return &subAxis();
			else
				return &channels.at(ChannelId::size);
		}
		else
			return nullptr;

	default: return nullptr;
	}
}

ChannelId Options::stackAxisType() const
{
	if (channels.anyAxisSet()) {
		switch ((ShapeType::Type)shapeType.get()) {
		case ShapeType::Type::Area:
		case ShapeType::Type::Rectangle: return subAxisType();
		default:
		case ShapeType::Type::Circle:
		case ShapeType::Type::Line: return ChannelId::size;
		}
	}
	else
		return ChannelId::size;
}

std::optional<ChannelId> Options::secondaryStackType() const
{
	if (channels.anyAxisSet() && shapeType.get() == ShapeType::Line)
		return subAxisType();

	return std::nullopt;
}

Channels Options::shadowChannels() const
{
	auto shadow = channels.shadow();

	std::vector<Vizzu::Gen::ChannelId> stackChannels;
	stackChannels.push_back(stackAxisType());
	auto secondary = secondaryStackType();
	if (secondary) stackChannels.push_back(*secondary);

	auto stackers = shadow.getDimensions(stackChannels);

	for (auto &stacker : stackers) {
		shadow.removeSeries(stackAxisType(), stacker);
		shadow.removeSeries(ChannelId::noop, stacker);
	}
	if (shadow.at(stackAxisType()).measureId()->getType()
	    == Data::SeriesType::Exists)
		shadow.at(stackAxisType()).clearMeasure();

	return shadow;
}

void Options::drilldownTo(const Options &other)
{
	auto &stackAxis = this->stackAxis();

	auto dimensions = other.getChannels().getDimensions();

	for (const auto &dim : dimensions)
		if (!getChannels().isSeriesUsed(dim)) stackAxis.addSeries(dim);
}

void Options::intersection(const Options &other)
{
	auto dimensions = getChannels().getDimensions();

	for (const auto &dim : dimensions)
		if (!other.getChannels().isSeriesUsed(dim)) {
			getChannels().removeSeries(dim);
		}
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
	else
		return *this == other;
}

void Options::simplify()
{
	//	remove all dimensions, only used at the end of stack
	auto &stackAxis = this->stackAxis();

	auto dimensions = stackAxis.dimensionIds();

	auto copy = getChannels();
	copy.at(stackAxisType()).reset();

	auto dim = dimensions.rbegin();
	for (; dim != dimensions.rend(); ++dim) {
		if (!copy.isSeriesUsed(*dim))
			stackAxis.removeSeries(*dim);
		else
			break;
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
	auto shape = shapeType.get();
	if (shape == ShapeType::Line) shape = ShapeType::Area;

	auto shapeOther = other.shapeType.get();
	if (shapeOther == ShapeType::Line) shapeOther = ShapeType::Area;

	return shape == shapeOther && polar.get() == other.polar.get()
	    && angle.get() == other.angle.get()
	    && horizontal.get() == other.horizontal.get()
	    && splitted.get() == other.splitted.get()
	    && dataFilter.get() == other.dataFilter.get()
	    && alignType.get() == other.alignType.get()
	    && splitted.get() == other.splitted.get()
	    && sorted.get() == other.sorted.get()
	    && reverse.get() == other.reverse.get();
}

bool Options::sameAttributes(const Options &other) const
{
	return sameShadowAttribs(other)
	    && shapeType.get() == other.shapeType.get()
	    && title.get() == other.title.get()
	    && legend.get() == other.legend.get()
	    && markersInfo.get() == other.markersInfo.get();
}

ChannelId Options::getHorizontalChannel() const
{
	return (Math::rad2quadrant(angle.get()) % 2) == 0 ? ChannelId::x
	                                                  : ChannelId::y;
}

ChannelId Options::getVerticalChannel() const
{
	return getHorizontalChannel() == ChannelId::x ? ChannelId::y
	                                          : ChannelId::x;
}

bool Options::isShapeValid(const ShapeType::Type &shapeType) const
{
	if (channels.anyAxisSet() && mainAxis().dimensionCount() > 0)
		return true;
	else
		return shapeType == ShapeType::Rectangle
		    || shapeType == ShapeType::Circle;
}

uint64_t Options::getMarkerInfoId(MarkerId id) const
{
	for (auto &i : markersInfo.get()) {
		if (i.second == id) return i.first;
	}
	return nullMarkerInfoId;
}

uint64_t Options::generateMarkerInfoId() const
{
	return nextMarkerInfoId++;
}

void Options::setAutoParameters()
{
	if (legend.get().get().isAuto()) {
		Base::AutoParam<ChannelId> tmp = legend.get().get();
		tmp.setAuto(getAutoLegend());
		legend.set(tmp);
	}
}

std::optional<ChannelId> Options::getAutoLegend()
{
	auto series = channels.getDimensions();
	series.merge(channels.getSeries());

	for (auto id : channels.at(ChannelId::label).dimensionIds())
		series.erase(id);

	if (!channels.at(ChannelId::label).isPseudoDimension())
		series.erase(*channels.at(ChannelId::label).measureId());

	for (auto channelId : {ChannelId::x, ChannelId::y}) {
		auto id = channels.at(channelId).labelSeries();
		if (id) series.erase(*id);
	}

	for (auto channelId : {ChannelId::color, ChannelId::lightness})
		for (auto id : channels.at(channelId).dimensionIds())
			if (series.contains(id)) return channelId;

	for (auto channelId :
	    {ChannelId::color, ChannelId::lightness, ChannelId::size})
		if (!channels.at(channelId).isPseudoDimension())
			if (series.contains(*channels.at(channelId).measureId()))
				return channelId;

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
	else if (!(bool)polar.get()) {
		if (!h.isPseudoDimension() && !v.isPseudoDimension()
		    && shapeType.get() == ShapeType::Rectangle) {
			setRange(h, 0.0_perc, 100.0_perc);
			setRange(v, 0.0_perc, 100.0_perc);
		}
		else {
			if (h.isPseudoDimension())
				setRange(h, 0.0_perc, 100.0_perc);
			else
				setMeasureRange(h, hPositive);

			if (v.isPseudoDimension())
				setRange(v, 0.0_perc, 100.0_perc);
			else
				setMeasureRange(v, vPositive);
		}
	}
	else {
		if (!h.isPseudoDimension() && v.isPseudoDimension()) {
			if (v.isEmpty()) {
				setRange(h, 0.0_perc, 100.0_perc);
				setRange(v, 0.0_perc, 100.0_perc);
			}
			else {
				setRange(h, 0.0_perc, 133.0_perc);
				setRange(v, 0.0_perc, 100.0_perc);
			}
		}
		else if (h.isPseudoDimension() && !v.isPseudoDimension()) {
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
	if (channel.range.ref().max.isAuto())
		channel.range.ref().max.setAuto(max);

	if (channel.range.ref().min.isAuto())
		channel.range.ref().min.setAuto(min);
}
