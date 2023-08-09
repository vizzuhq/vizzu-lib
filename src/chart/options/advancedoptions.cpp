#include "advancedoptions.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

OptionsSetter &OrientationSelector::addSeries(const ChannelId &channelId,
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	Base::addSeries(channelId, index, pos);
	fixHorizontal();
	return *this;
}

OptionsSetter &OrientationSelector::deleteSeries(
    const ChannelId &channelId,
    const Data::SeriesIndex &index)
{
	Base::deleteSeries(channelId, index);
	fixHorizontal();
	return *this;
}

OptionsSetter &OrientationSelector::setHorizontal(bool horizontal)
{
	auto horOver = horizontalOverride();
	Base::setHorizontal(horOver ? *horOver : horizontal);
	return *this;
}

OptionsSetter &OrientationSelector::setShape(
    const ShapeType &type)
{
	Base::setShape(type);
	fixHorizontal();
	return *this;
}

void OrientationSelector::fixHorizontal()
{
	auto horOver = horizontalOverride();
	if (horOver) Base::setHorizontal(*horOver);
}

std::optional<bool> OrientationSelector::horizontalOverride() const
{
	if (options.getChannels().anyAxisSet()
	    && options.geometry != ShapeType::circle) {
		auto &x = options.getChannels().at(ChannelId::x);
		auto &y = options.getChannels().at(ChannelId::y);

		if (x.isEmpty() && !y.isDimension()) return true;
		if (y.isEmpty() && !x.isDimension()) return false;

		if (!x.dimensionIds.empty() && y.dimensionIds.empty()
		    && !y.isDimension())
			return true;
		if (!y.dimensionIds.empty() && x.dimensionIds.empty()
		    && !x.isDimension())
			return false;

		if (!x.dimensionIds.empty() && !y.dimensionIds.empty()) {
			if (x.isDimension() && !y.isDimension())
				return true;
			if (y.isDimension() && !x.isDimension())
				return false;
		}
	}
	return std::nullopt;
}
