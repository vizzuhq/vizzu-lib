#include "autosplitter.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

OptionsSetter &AutoSplitter::addSeries(const Scales::Id &scaleId,
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	if (options.shapeType.get() == ShapeType::Type::Rectangle
	    && index.getType().isDiscrete())
	{
		if (scaleId.type == options.mainAxisType()
		    && !options.subAxisOf(scaleId)->isSeriesUsed(index))
		{
			auto otherId =
			    Scales::Id{options.subAxisType(), Scales::Index{0}};

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
		auto subId = Scales::Id{Scale::Size, Scales::Index{0}};

		if (scaleId.type != subId.type
		    && !options.getScales().isSeriesUsed(index))
		{
			setter.addSeries(subId, index);
			onFinished();
			setter.addSeries(scaleId, index, pos);
			setter.deleteSeries(subId, index);
		}
		else
			setter.addSeries(scaleId, index, pos);
	}
	else if (options.shapeType.get() == ShapeType::Type::Line
	         && index.getType().isDiscrete())
	{
		auto subId = Scales::Id{Scale::Size, Scales::Index{0}};

		if (scaleId.type != subId.type
		    && !options.getScales().isSeriesUsed(index))
		{
			setter.addSeries(subId, index);
			if (scaleId.type != options.mainAxisType()) onFinished();
			setter.addSeries(scaleId, index, pos);
			if (scaleId.type == options.mainAxisType()) onFinished();
			setter.deleteSeries(subId, index);
		}
		else
			setter.addSeries(scaleId, index, pos);
	}
	else if (options.shapeType.get() == ShapeType::Type::Area
	         && index.getType().isDiscrete())
	{
		if (scaleId.type == options.mainAxisType()
		    && !options.subAxisOf(scaleId)->isSeriesUsed(index))
		{
			auto otherId =
			    Scales::Id{options.subAxisType(), Scales::Index{0}};

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

OptionsSetter &AutoSplitter::deleteSeries(const Scales::Id &scaleId,
    const Data::SeriesIndex &index)
{
	if (options.shapeType.get() == ShapeType::Type::Rectangle
	    && index.getType().isDiscrete())
	{
		if (scaleId.type == options.mainAxisType()
		    && !options.subAxis(Scales::Index{0}).isSeriesUsed(index))
		{
			auto otherId = scaleId;
			otherId.type = options.subAxisType();

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
		auto subType = Scale::Size;
		auto subId = Scales::Id{subType, Scales::Index{0}};

		if (scaleId.type != subType && index.getType().isDiscrete()
		    && options.getScales().count(index) == 1)
		{
			setter.addSeries(subId, index);
			setter.deleteSeries(scaleId, index);
			onFinished();
			setter.deleteSeries(subId, index);
		}
		else
			setter.deleteSeries(scaleId, index);
	}
	else if (options.shapeType.get() == ShapeType::Type::Line)
	{
		auto subType = Scale::Size;
		auto subId = Scales::Id{subType, Scales::Index{0}};

		if (scaleId.type != subType && index.getType().isDiscrete()
		    && options.getScales().count(index) == 1)
		{
			setter.addSeries(subId, index);
			if (scaleId.type == options.mainAxisType()) onFinished();
			setter.deleteSeries(scaleId, index);
			if (scaleId.type != options.mainAxisType()) onFinished();
			setter.deleteSeries(subId, index);
		}
		else
			setter.deleteSeries(scaleId, index);
	}
	else if (options.shapeType.get() == ShapeType::Type::Area
	         && index.getType().isDiscrete())
	{
		if (scaleId.type == options.mainAxisType()
		    && !options.subAxis(Scales::Index{0}).isSeriesUsed(index))
		{
			auto otherId = scaleId;
			otherId.type = options.subAxisType();

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
