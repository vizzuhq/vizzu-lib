#include "advancedoptions.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

OptionsSetter &ExistsHandler::addSeries(const ScaleId &scaleId,
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	Base::addSeries(scaleId, index, pos);
	handleExists();
	return *this;
}

OptionsSetter &ExistsHandler::deleteSeries(const ScaleId &scaleId,
    const Data::SeriesIndex &index)
{
	Base::deleteSeries(scaleId, index);
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
		options.getScales().visitAll(
		    [=, this](ScaleId id, const Scale &scale)
		    {
			    if (scale.discretesIds().empty()
			        && scale.continousId()
			        && scale.continousId()->getType()
			               == Data::SeriesType::Exists)
				    Base::deleteSeries(id,
				        Data::SeriesIndex(Data::SeriesType::Exists));

			    if ((Gen::isAxis(id) || id == ScaleId::size)
			        && !scale.isEmpty() && !scale.continousId()) {
				    Base::addSeries(id,
				        Data::SeriesIndex(Data::SeriesType::Exists));
			    }
		    });
}

OptionsSetter &AdvancedOptions::deleteSeries(const ScaleId &scaleId,
    const Data::SeriesIndex &index)
{
	Base::deleteSeries(scaleId, index);

	if (!options.getScales().anyAxisSet()
	    && ((ShapeType::Type)options.shapeType.get()
	            != ShapeType::Circle
	        || (ShapeType::Type)options.shapeType.get()
	               != ShapeType::Circle)) {
		Base::setShape(ShapeType::Rectangle);
	}
	return *this;
}

OptionsSetter &OrientationSelector::addSeries(const ScaleId &scaleId,
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	Base::addSeries(scaleId, index, pos);
	fixHorizontal();
	return *this;
}

OptionsSetter &OrientationSelector::deleteSeries(
    const ScaleId &scaleId,
    const Data::SeriesIndex &index)
{
	Base::deleteSeries(scaleId, index);
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
	if (options.getScales().anyAxisSet()
	    && options.shapeType.get().type() != ShapeType::Circle) {
		auto &x = options.getScales().at(ScaleId::x);
		auto &y = options.getScales().at(ScaleId::y);

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
