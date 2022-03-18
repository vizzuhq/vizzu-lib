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

	OptionsSetter &addSeries(const ScaleId &scaleId, const Data::SeriesIndex &index,
							 std::optional<size_t> pos = std::nullopt) override;
	OptionsSetter &deleteSeries(const ScaleId &scaleId, const Data::SeriesIndex &index) override;
	OptionsSetter &setHorizontal(bool horizontal) override;
private:
	std::optional<bool> horizontalOverride() const;
	void fixHorizontal();
};

class AdvancedOptions : public OrientationSelector
{
public:
	using OrientationSelector::OrientationSelector;
	typedef OrientationSelector Base;

	OptionsSetter &deleteSeries(const ScaleId &scaleId, const Data::SeriesIndex &index) override;
};

}
}

#endif
