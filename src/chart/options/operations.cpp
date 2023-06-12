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

	auto subId = options.subAxisType();
	auto mainId = options.mainAxisType();

	const auto &subAxis = options.subAxis();
	const auto &mainAxis = options.mainAxis();

	if (index.getType().isContinous()) {
		if (subAxis.isPseudoDiscrete()) {
			setter->addSeries(subId, index);
		}
		else if (mainAxis.isPseudoDiscrete()) {
			setter->addSeries(mainId, index);
		}
		else if (options.getChannels()
		             .at(ChannelId::size)
		             .isPseudoDiscrete()) {
			if (!Diag::canOverlap(
			        (ShapeType::Type)options.shapeType.get()))
				setter->setShape(ShapeType::Circle);
			setter->addSeries(ChannelId::size, index);
		}
	}
	else {
		if (Diag::canOverlap(
		        (ShapeType::Type)options.shapeType.get())) {
			setter->addSeries(ChannelId::label, index);
		}
		else {
			setter->addSeries(subId, index);
		}
	}
}

void Operations::removeSeries(const Data::SeriesIndex &index)
{
	const auto &options = setter->getOptions();

	options.getChannels().visitAll(
	    [=, this](ChannelId id, const Channel &channel)
	    {
		    if (channel.isSeriesUsed(index))
			    setter->deleteSeries(id, index);
	    });
}

void Operations::split()
{
	const auto &options = setter->getOptions();
	if (options.shapeType.get() == ShapeType::Rectangle
	    && options.getChannels().anyAxisSet()) {
		auto mainId = options.mainAxisType();
		auto subId = options.subAxisType();
		split(mainId, subId);
	}
	if (options.shapeType.get() == ShapeType::Area
	    && options.getChannels().anyAxisSet()) {
		setter->setSplitted(true);
	}
	if (options.shapeType.get() == ShapeType::Circle
	    || options.shapeType.get() == ShapeType::Line
	    || !options.getChannels().anyAxisSet()) {
		split(ChannelId::label, ChannelId::size);
	}
}

void Operations::split(const ChannelId &mainId, const ChannelId &subId)
{
	const auto &options = setter->getOptions();
	const auto &sub = options.getChannels().at(subId);

	if (!sub.discretesIds().empty()) {
		const auto &series = *sub.discretesIds().begin();

		setter->addSeries(mainId, series);
		setter->deleteSeries(subId, series);
	}
}

void Operations::stack()
{
	const auto &options = setter->getOptions();
	if (options.getChannels().anyAxisSet()
	    && options.shapeType.get() == ShapeType::Rectangle) {
		auto mainId = options.mainAxisType();
		auto subId = options.subAxisType();
		stack(mainId, subId);
	}
	if (options.shapeType.get() == ShapeType::Area
	    && options.getChannels().anyAxisSet()) {
		setter->setSplitted(false);
	}
	if (options.shapeType.get() == ShapeType::Circle
	    || options.shapeType.get() == ShapeType::Line
	    || !options.getChannels().anyAxisSet()) {
		stack(ChannelId::label, ChannelId::size);
	}
}

void Operations::stack(const ChannelId &mainId, const ChannelId &subId)
{
	const auto &options = setter->getOptions();
	const auto &main = options.getChannels().at(mainId);

	if (!main.discretesIds().empty()) {
		auto series = *main.discretesIds().rbegin();

		setter->addSeries(subId, series, 0);
		setter->deleteSeries(mainId, series);
	}
}

void Operations::fit(bool /* enable */) {}

void Operations::swapDimension()
{
	const auto &options = setter->getOptions();

	if (!options.getChannels().anyAxisSet()) return;

	if (options.shapeType.get() == ShapeType::Rectangle) {
		auto subIsCont = !options.subAxis().isPseudoDiscrete();
		auto mainIsCont = !options.mainAxis().isPseudoDiscrete();

		if (subIsCont && mainIsCont) {
			const auto &options = setter->getOptions();
			setter->setHorizontal(!(bool)options.horizontal.get());
		}
		else if (subIsCont && !mainIsCont) {
			auto cont = *options.subAxis().continousId();
			setter->deleteSeries(options.subAxisType(), cont);
			setter->addSeries(options.mainAxisType(), cont);
		}
		else if (!subIsCont && mainIsCont) {
			auto cont = *options.mainAxis().continousId();
			setter->deleteSeries(options.mainAxisType(), cont);
			setter->addSeries(options.subAxisType(), cont);
		}
		else {
			const auto &options = setter->getOptions();
			setter->setHorizontal(!(bool)options.horizontal.get());
		}
	}
	else {
		const auto &options = setter->getOptions();
		setter->setHorizontal(!(bool)options.horizontal.get());
	}
}

bool Operations::isFit() const
{
	const auto &options = setter->getOptions();

	if (options.shapeType.get() == ShapeType::Rectangle) {
		const auto &main = options.mainAxis();
		const auto &sub = options.subAxis();

		if (main.continousId()
		    && (options.alignType.get() == Base::Align::Fit
		        || main.discretesIds().empty()
		        || !sub.continousId())) {
			return true;
		}
	}
	return false;
}
