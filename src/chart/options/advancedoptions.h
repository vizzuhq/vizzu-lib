#ifndef ADVANCEDOPTIONS_H
#define ADVANCEDOPTIONS_H

#include "optionssetter.h"

namespace Vizzu
{
namespace Diag
{

class OrientationSelector : public OptionsSetter
{
public:
	typedef OptionsSetter Base;
	using OptionsSetter::OptionsSetter;

	OptionsSetter &addSeries(const ScaleId &scaleId,
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt) override;
	OptionsSetter &deleteSeries(const ScaleId &scaleId,
	    const Data::SeriesIndex &index) override;
	OptionsSetter &setHorizontal(bool horizontal) override;
	OptionsSetter &setShape(const ShapeType::Type &type) override;

private:
	std::optional<bool> horizontalOverride() const;
	void fixHorizontal();
};

class ExistsHandler : public OrientationSelector
{
public:
	typedef OrientationSelector Base;

	bool forcedExistsSeries = true;

	using OrientationSelector::OrientationSelector;

	OptionsSetter &addSeries(const ScaleId &scaleId,
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt) override;
	OptionsSetter &deleteSeries(const ScaleId &scaleId,
	    const Data::SeriesIndex &index) override;
	OptionsSetter &setHorizontal(bool horizontal) override;

private:
	void handleExists();
};

class AdvancedOptions : public ExistsHandler
{
public:
	using ExistsHandler::ExistsHandler;
	typedef ExistsHandler Base;

	OptionsSetter &deleteSeries(const ScaleId &scaleId,
	    const Data::SeriesIndex &index) override;
};

}
}

#endif
