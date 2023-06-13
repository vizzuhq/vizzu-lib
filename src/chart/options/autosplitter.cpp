#include "autosplitter.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

OptionsSetter &AutoSplitter::addSeries(const ChannelId &channelId,
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	if (options.shapeType.get() == ShapeType::Type::Rectangle
	    && index.getType().isDimension()) {
		if (channelId == options.mainAxisType()
		    && !options.subAxisOf(channelId)->isSeriesUsed(index)) {
			auto otherId = options.subAxisType();
			setter.addSeries(otherId, index);
			onFinished();
			setter.addSeries(channelId, index, pos);
			if (options.shapeType.get() != ShapeType::Rectangle)
				onFinished();
			setter.deleteSeries(otherId, index);
		}
		else
			setter.addSeries(channelId, index, pos);
	}
	else if (options.shapeType.get() == ShapeType::Type::Circle
	         && index.getType().isDimension()) {
		if (channelId != ChannelId::size
		    && !options.getChannels().isSeriesUsed(index)) {
			setter.addSeries(ChannelId::size, index);
			onFinished();
			setter.addSeries(channelId, index, pos);
			setter.deleteSeries(ChannelId::size, index);
		}
		else
			setter.addSeries(channelId, index, pos);
	}
	else if (options.shapeType.get() == ShapeType::Type::Line
	         && index.getType().isDimension()) {
		if (channelId != ChannelId::size
		    && !options.getChannels().isSeriesUsed(index)) {
			setter.addSeries(ChannelId::size, index);
			if (channelId != options.mainAxisType()) onFinished();
			setter.addSeries(channelId, index, pos);
			if (channelId == options.mainAxisType()) onFinished();
			setter.deleteSeries(ChannelId::size, index);
		}
		else
			setter.addSeries(channelId, index, pos);
	}
	else if (options.shapeType.get() == ShapeType::Type::Area
	         && index.getType().isDimension()) {
		if (channelId == options.mainAxisType()
		    && !options.subAxisOf(channelId)->isSeriesUsed(index)) {
			auto otherId = options.subAxisType();
			setter.addSeries(otherId, index);
			setter.addSeries(channelId, index, pos);
			onFinished();
			setter.deleteSeries(otherId, index);
		}
		else
			setter.addSeries(channelId, index, pos);
	}
	else
		setter.addSeries(channelId, index, pos);

	return *this;
}

OptionsSetter &AutoSplitter::deleteSeries(const ChannelId &channelId,
    const Data::SeriesIndex &index)
{
	if (options.shapeType.get() == ShapeType::Type::Rectangle
	    && index.getType().isDimension()) {
		if (channelId == options.mainAxisType()
		    && !options.subAxis().isSeriesUsed(index)) {
			auto otherId = channelId;
			otherId = options.subAxisType();

			setter.addSeries(otherId, index);
			if (options.shapeType.get() != ShapeType::Rectangle)
				onFinished();
			setter.deleteSeries(channelId, index);
			onFinished();
			setter.deleteSeries(otherId, index);
		}
		else
			setter.deleteSeries(channelId, index);
	}
	else if (options.shapeType.get() == ShapeType::Type::Circle) {
		if (channelId != ChannelId::size && index.getType().isDimension()
		    && options.getChannels().count(index) == 1) {
			setter.addSeries(ChannelId::size, index);
			setter.deleteSeries(channelId, index);
			onFinished();
			setter.deleteSeries(ChannelId::size, index);
		}
		else
			setter.deleteSeries(channelId, index);
	}
	else if (options.shapeType.get() == ShapeType::Type::Line) {
		if (channelId != ChannelId::size && index.getType().isDimension()
		    && options.getChannels().count(index) == 1) {
			setter.addSeries(ChannelId::size, index);
			if (channelId == options.mainAxisType()) onFinished();
			setter.deleteSeries(channelId, index);
			if (channelId != options.mainAxisType()) onFinished();
			setter.deleteSeries(ChannelId::size, index);
		}
		else
			setter.deleteSeries(channelId, index);
	}
	else if (options.shapeType.get() == ShapeType::Type::Area
	         && index.getType().isDimension()) {
		if (channelId == options.mainAxisType()
		    && !options.subAxis().isSeriesUsed(index)) {
			auto otherId = channelId;
			otherId = options.subAxisType();

			setter.addSeries(otherId, index);
			onFinished();
			setter.deleteSeries(channelId, index);
			setter.deleteSeries(otherId, index);
		}
		else
			setter.deleteSeries(channelId, index);
	}
	else
		setter.deleteSeries(channelId, index);

	return *this;
}

OptionsSetter &AutoSplitter::setSplitted(bool split)
{
	if (split) {
		if (options.shapeType.get() == ShapeType::Area)
			setter.setSplitted(true);
	}
	else
		setter.setSplitted(false);

	return *this;
}

OptionsSetter &AutoSplitter::setShape(const ShapeType::Type &type)
{
	if (type != ShapeType::Area) setter.setSplitted(false);

	setter.setShape(type);

	return *this;
}
