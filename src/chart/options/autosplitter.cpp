#include "autosplitter.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

OptionsSetter &AutoSplitter::addSeries(const ScaleId &scaleId,
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	if (options.shapeType.get() == ShapeType::Type::Rectangle
	    && index.getType().isDiscrete())
	{
		if (scaleId == options.mainAxisType()
		    && !options.subAxisOf(scaleId)->isSeriesUsed(index))
		{
			auto otherId = options.subAxisType();
			setter.addSeries(otherId, index);
			onFinished();
			setter.addSeries(scaleId, index, pos);
			if (options.shapeType.get() != ShapeType::Rectangle)
				onFinished();
			setter.deleteSeries(otherId, index);
		}
		else
			setter.addSeries(scaleId, index, pos);
	}
	else if (options.shapeType.get() == ShapeType::Type::Circle
	         && index.getType().isDiscrete())
	{
		if (scaleId != ScaleId::size
		    && !options.getScales().isSeriesUsed(index))
		{
			setter.addSeries(ScaleId::size, index);
			onFinished();
			setter.addSeries(scaleId, index, pos);
			setter.deleteSeries(ScaleId::size, index);
		}
		else
			setter.addSeries(scaleId, index, pos);
	}
	else if (options.shapeType.get() == ShapeType::Type::Line
	         && index.getType().isDiscrete())
	{
		if (scaleId != ScaleId::size
		    && !options.getScales().isSeriesUsed(index))
		{
			setter.addSeries(ScaleId::size, index);
			if (scaleId != options.mainAxisType()) onFinished();
			setter.addSeries(scaleId, index, pos);
			if (scaleId == options.mainAxisType()) onFinished();
			setter.deleteSeries(ScaleId::size, index);
		}
		else
			setter.addSeries(scaleId, index, pos);
	}
	else if (options.shapeType.get() == ShapeType::Type::Area
	         && index.getType().isDiscrete())
	{
		if (scaleId == options.mainAxisType()
		    && !options.subAxisOf(scaleId)->isSeriesUsed(index))
		{
			auto otherId = options.subAxisType();
			setter.addSeries(otherId, index);
			setter.addSeries(scaleId, index, pos);
			onFinished();
			setter.deleteSeries(otherId, index);
		}
		else
			setter.addSeries(scaleId, index, pos);
	}
	else
		setter.addSeries(scaleId, index, pos);

	return *this;
}

OptionsSetter &AutoSplitter::deleteSeries(const ScaleId &scaleId,
    const Data::SeriesIndex &index)
{
	if (options.shapeType.get() == ShapeType::Type::Rectangle
	    && index.getType().isDiscrete())
	{
		if (scaleId == options.mainAxisType()
		    && !options.subAxis().isSeriesUsed(index))
		{
			auto otherId = scaleId;
			otherId = options.subAxisType();

			setter.addSeries(otherId, index);
			if (options.shapeType.get() != ShapeType::Rectangle)
				onFinished();
			setter.deleteSeries(scaleId, index);
			onFinished();
			setter.deleteSeries(otherId, index);
		}
		else
			setter.deleteSeries(scaleId, index);
	}
	else if (options.shapeType.get() == ShapeType::Type::Circle)
	{
		if (scaleId != ScaleId::size && index.getType().isDiscrete()
		    && options.getScales().count(index) == 1)
		{
			setter.addSeries(ScaleId::size, index);
			setter.deleteSeries(scaleId, index);
			onFinished();
			setter.deleteSeries(ScaleId::size, index);
		}
		else
			setter.deleteSeries(scaleId, index);
	}
	else if (options.shapeType.get() == ShapeType::Type::Line)
	{
		if (scaleId != ScaleId::size && index.getType().isDiscrete()
		    && options.getScales().count(index) == 1)
		{
			setter.addSeries(ScaleId::size, index);
			if (scaleId == options.mainAxisType()) onFinished();
			setter.deleteSeries(scaleId, index);
			if (scaleId != options.mainAxisType()) onFinished();
			setter.deleteSeries(ScaleId::size, index);
		}
		else
			setter.deleteSeries(scaleId, index);
	}
	else if (options.shapeType.get() == ShapeType::Type::Area
	         && index.getType().isDiscrete())
	{
		if (scaleId == options.mainAxisType()
		    && !options.subAxis().isSeriesUsed(index))
		{
			auto otherId = scaleId;
			otherId = options.subAxisType();

			setter.addSeries(otherId, index);
			onFinished();
			setter.deleteSeries(scaleId, index);
			setter.deleteSeries(otherId, index);
		}
		else
			setter.deleteSeries(scaleId, index);
	}
	else
		setter.deleteSeries(scaleId, index);

	return *this;
}

OptionsSetter &AutoSplitter::setSplitted(bool split)
{
	if (split)
	{
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
