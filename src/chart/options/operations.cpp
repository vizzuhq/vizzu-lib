#include "operations.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

Operations::Operations(OptionsSetterPtr setter,
    const Data::DataTable &table) :
    setter(std::move(setter)),
    table(table)
{}

void Operations::addSeries(const Data::SeriesIndex &index)
{
	const auto &options = setter->getOptions();

	auto subId =
		Scales::Id{options.subAxisType(), Scales::Index{0}};
	auto mainId =
		Scales::Id{options.mainAxisType(), Scales::Index{0}};
	auto sizeId = Scales::Id{ScaleId::size, Scales::Index{0}};

	const auto &subAxis = options.subAxis(Scales::Index{0});
	const auto &mainAxis = options.mainAxis(Scales::Index{0});

	if (index.getType().isContinous())
	{
		if (subAxis.isPseudoDiscrete())
		{
			setter->addSeries(subId, index);
		}
		else if (mainAxis.isPseudoDiscrete())
		{
			setter->addSeries(mainId, index);
		}
		else if (options.getScales()
						.at(sizeId)
						.isPseudoDiscrete())
		{
			if (!Diag::canOverlap(
					(ShapeType::Type)options.shapeType.get()))
				setter->setShape(ShapeType::Circle);
			setter->addSeries(sizeId, index);
		}
	}
	else
	{
		if (Diag::canOverlap(
				(ShapeType::Type)options.shapeType.get()))
		{
			auto targetId =
				Scales::Id{ScaleId::label, Scales::Index{0}};
			setter->addSeries(targetId, index);
		}
		else
		{
			setter->addSeries(subId, index);
		}
	}
}

void Operations::removeSeries(const Data::SeriesIndex &index)
{
	const auto &options = setter->getOptions();

	options.getScales().visitAll(
	    [=](Scales::Id id, const Scale &scale)
	    {
		    if (scale.isSeriesUsed(index))
			    setter->deleteSeries(id, index);
	    });
}

void Operations::split()
{
	const auto &options = setter->getOptions();
	if (options.shapeType.get() == ShapeType::Rectangle
	    && options.getScales().anyAxisSet())
	{
		auto mainId =
		    Scales::Id{options.mainAxisType(), Scales::Index{0}};
		auto subId =
		    Scales::Id{options.subAxisType(), Scales::Index{0}};
		split(mainId, subId);
	}
	if (options.shapeType.get() == ShapeType::Area
	    && options.getScales().anyAxisSet())
	{
		setter->setSplitted(true);
	}
	if (options.shapeType.get() == ShapeType::Circle
	    || options.shapeType.get() == ShapeType::Line
	    || !options.getScales().anyAxisSet())
	{
		auto mainId =
		    Scales::Id{ScaleId::label, Scales::Index{0}};
		auto subId = Scales::Id{ScaleId::size, Scales::Index{0}};
		split(mainId, subId);
	}
}

void Operations::split(const Scales::Id &mainId,
    const Scales::Id &subId)
{
	const auto &options = setter->getOptions();
	const auto &sub = options.getScales().at(subId);

	if (!sub.discretesIds().empty())
	{
		const auto &series = *sub.discretesIds().begin();

		setter->addSeries(mainId, series);
		setter->deleteSeries(subId, series);
	}
}

void Operations::stack()
{
	const auto &options = setter->getOptions();
	if (options.getScales().anyAxisSet()
	    && options.shapeType.get() == ShapeType::Rectangle)
	{
		auto mainId =
		    Scales::Id{options.mainAxisType(), Scales::Index{0}};
		auto subId =
		    Scales::Id{options.subAxisType(), Scales::Index{0}};
		stack(mainId, subId);
	}
	if (options.shapeType.get() == ShapeType::Area
	    && options.getScales().anyAxisSet())
	{
		setter->setSplitted(false);
	}
	if (options.shapeType.get() == ShapeType::Circle
	    || options.shapeType.get() == ShapeType::Line
	    || !options.getScales().anyAxisSet())
	{
		auto mainId =
		    Scales::Id{ScaleId::label, Scales::Index{0}};
		auto subId = Scales::Id{ScaleId::size, Scales::Index{0}};
		stack(mainId, subId);
	}
}

void Operations::stack(const Scales::Id &mainId,
    const Scales::Id &subId)
{
	const auto &options = setter->getOptions();
	const auto &main = options.getScales().at(mainId);

	if (!main.discretesIds().empty())
	{
		auto series = *main.discretesIds().rbegin();

		setter->addSeries(subId, series, 0);
		setter->deleteSeries(mainId, series);
	}
}

void Operations::drillDown()
{
	const auto &options = setter->getOptions();
	auto dimensions = options.getScales().getDimensions();

	for (auto series : dimensions)
	{
		auto child =
		    table.getHierarchy().childOf(series.getColIndex());
		if (child)
		{
			auto childId = Data::SeriesIndex(table.getIndex(*child));
			if (!options.getScales().isSeriesUsed(childId))
				addSeries(childId);
		}
	}
}

void Operations::drillUp()
{
	const auto &options = setter->getOptions();
	auto dimensions = options.getScales().getDimensions();

	for (auto series : dimensions)
	{
		auto parent =
		    table.getHierarchy().parentOf(series.getColIndex());
		auto child =
		    table.getHierarchy().childOf(series.getColIndex());
		if (parent)
		{
			auto childUsed =
			    child
			    && options.getScales().isSeriesUsed(
			        Data::SeriesIndex(table.getIndex(*child)));

			auto parentId =
			    Data::SeriesIndex(table.getIndex(*parent));
			if (!childUsed
			    && options.getScales().isSeriesUsed(parentId))
			{
				removeSeries(series);
				return;
			}
		}
	}
}

void Operations::fit(bool /* enable */) {}

void Operations::swapDimension()
{
	const auto &options = setter->getOptions();

	if (!options.getScales().anyAxisSet()) return;

	if (options.shapeType.get() == ShapeType::Rectangle)
	{
		auto subIsCont = !options.subAxis().isPseudoDiscrete();
		auto mainIsCont = !options.mainAxis().isPseudoDiscrete();

		if (subIsCont && mainIsCont)
		{
			const auto &options = setter->getOptions();
			setter->setHorizontal(!(bool)options.horizontal.get());
		}
		else if (subIsCont && !mainIsCont)
		{
			auto cont = *options.subAxis().continousId();
			setter->deleteSeries(options.subAxisId(), cont);
			setter->addSeries(options.mainAxisId(), cont);
		}
		else if (!subIsCont && mainIsCont)
		{
			auto cont = *options.mainAxis().continousId();
			setter->deleteSeries(options.mainAxisId(), cont);
			setter->addSeries(options.subAxisId(), cont);
		}
		else
		{
			const auto &options = setter->getOptions();
			setter->setHorizontal(!(bool)options.horizontal.get());
		}
	}
	else
	{
		const auto &options = setter->getOptions();
		setter->setHorizontal(!(bool)options.horizontal.get());
	}
}

bool Operations::isFit() const
{
	const auto &options = setter->getOptions();

	if (options.shapeType.get() == ShapeType::Rectangle)
	{
		const auto &main = options.mainAxis(Scales::Index{0});
		const auto &sub = options.subAxis(Scales::Index{0});

		if (main.continousId()
		    && (options.alignType.get() == Base::Align::Fit
		        || main.discretesIds().empty() || !sub.continousId()))
		{
			return true;
		}
	}
	return false;
}
