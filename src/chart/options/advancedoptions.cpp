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
			    if (scale.dimensionIds().empty()
			        && scale.measureId()
			        && scale.measureId()->getType()
			               == Data::SeriesType::Exists)
				    Base::deleteSeries(id,
				        Data::SeriesIndex(Data::SeriesType::Exists));

			    if ((Gen::isAxis(id) || id == ScaleId::size)
			        && !scale.isEmpty() && !scale.measureId()) {
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

		if (x.isEmpty() && !y.isPseudoDimension()) return true;
		if (y.isEmpty() && !x.isPseudoDimension()) return false;

		if (!x.dimensionIds().empty() && y.dimensionIds().empty()
		    && !y.isPseudoDimension())
			return true;
		if (!y.dimensionIds().empty() && x.dimensionIds().empty()
		    && !x.isPseudoDimension())
			return false;

		if (!x.dimensionIds().empty() && !y.dimensionIds().empty()) {
			if (x.isPseudoDimension() && !y.isPseudoDimension())
				return true;
			if (y.isPseudoDimension() && !x.isPseudoDimension())
				return false;
		}
	}
	return std::nullopt;
}
