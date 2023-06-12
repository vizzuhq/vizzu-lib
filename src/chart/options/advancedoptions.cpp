#include "advancedoptions.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

OptionsSetter &ExistsHandler::addSeries(const ChannelId &channelId,
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	Base::addSeries(channelId, index, pos);
	handleExists();
	return *this;
}

OptionsSetter &ExistsHandler::deleteSeries(const ChannelId &channelId,
    const Data::SeriesIndex &index)
{
	Base::deleteSeries(channelId, index);
	handleExists();
	return *this;
}

OptionsSetter &ExistsHandler::setHorizontal(bool horizontal)
{
	Base::setHorizontal(horizontal);
	handleExists();
	return *this;
}

void ExistsHandler::handleExists()
{
	if (forcedExistsSeries)
		options.getChannels().visitAll(
		    [=, this](ChannelId id, const Channel &channel)
		    {
			    if (channel.discretesIds().empty()
			        && channel.continousId()
			        && channel.continousId()->getType()
			               == Data::SeriesType::Exists)
				    Base::deleteSeries(id,
				        Data::SeriesIndex(Data::SeriesType::Exists));

			    if ((Diag::isAxis(id) || id == ChannelId::size)
			        && !channel.isEmpty() && !channel.continousId()) {
				    Base::addSeries(id,
				        Data::SeriesIndex(Data::SeriesType::Exists));
			    }
		    });
}

OptionsSetter &AdvancedOptions::deleteSeries(const ChannelId &channelId,
    const Data::SeriesIndex &index)
{
	Base::deleteSeries(channelId, index);

	if (!options.getChannels().anyAxisSet()
	    && ((ShapeType::Type)options.shapeType.get()
	            != ShapeType::Circle
	        || (ShapeType::Type)options.shapeType.get()
	               != ShapeType::Circle)) {
		Base::setShape(ShapeType::Rectangle);
	}
	return *this;
}

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
    const ShapeType::Type &type)
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
	    && options.shapeType.get().type() != ShapeType::Circle) {
		auto &x = options.getChannels().at(ChannelId::x);
		auto &y = options.getChannels().at(ChannelId::y);

		if (x.isEmpty() && !y.isPseudoDiscrete()) return true;
		if (y.isEmpty() && !x.isPseudoDiscrete()) return false;

		if (!x.discretesIds().empty() && y.discretesIds().empty()
		    && !y.isPseudoDiscrete())
			return true;
		if (!y.discretesIds().empty() && x.discretesIds().empty()
		    && !x.isPseudoDiscrete())
			return false;

		if (!x.discretesIds().empty() && !y.discretesIds().empty()) {
			if (x.isPseudoDiscrete() && !y.isPseudoDiscrete())
				return true;
			if (y.isPseudoDiscrete() && !x.isPseudoDiscrete())
				return false;
		}
	}
	return std::nullopt;
}
