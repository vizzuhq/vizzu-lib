#ifndef ADVANCEDOPTIONS_H
#define ADVANCEDOPTIONS_H

#include "optionssetter.h"

namespace Vizzu
{
namespace Gen
{

class OrientationSelector : public OptionsSetter
{
public:
	typedef OptionsSetter Base;
	using OptionsSetter::OptionsSetter;

	OptionsSetter &addSeries(const ChannelId &channelId,
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt) override;
	OptionsSetter &deleteSeries(const ChannelId &channelId,
	    const Data::SeriesIndex &index) override;
	OptionsSetter &setHorizontal(bool horizontal) override;
	OptionsSetter &setShape(const ShapeType::Type &type) override;

private:
	std::optional<bool> horizontalOverride() const;
	void fixHorizontal();
};

}
}

#endif
