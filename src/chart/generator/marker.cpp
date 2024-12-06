#include "marker.h"

#include <cmath>
#include <cstdint>
#include <optional>
#include <utility>

#include "base/conv/auto_json.h"
#include "base/geom/orientation.h"
#include "base/geom/point.h"
#include "base/geom/rect.h"
#include "base/math/range.h"
#include "base/refl/auto_enum.h"
#include "chart/options/align.h"
#include "chart/options/channel.h"
#include "chart/options/coordsystem.h"
#include "chart/options/shapetype.h"
#include "dataframe/old/datatable.h"
#include "dataframe/old/types.h"

#include "axis.h"

namespace Vizzu::Gen
{

Marker::Marker(const Options &options,
    const Data::DataCube &data,
    ChannelStats &stats,
    const Data::SeriesList &mainAxisList,
    const Data::SeriesList &subAxisList,
    const Data::MultiIndex &index,
    bool needMarkerInfo) :
    cellInfo(data.cellInfo(index, needMarkerInfo)),
    mainId(data.getId(mainAxisList,
        options.dimLabelIndex(+options.mainAxisType()),
        index)),
    subId(data.getId(subAxisList,
        options.dimLabelIndex(+options.subAxisType()),
        index)),
    sizeId(data.getId(
        options.getChannels().at(ChannelId::size).dimensions(),
        options.dimLabelIndex(ChannelId::size),
        index)),
    idx{index.marker_id}
{
	const auto &channels = options.getChannels();
	auto color = getValueForChannel(channels,
	    ChannelId::color,
	    data,
	    stats,
	    index,
	    data.getId(channels.at(ChannelId::color).dimensions(),
	        options.dimLabelIndex(ChannelId::color),
	        index));

	auto lightness = getValueForChannel(channels,
	    ChannelId::lightness,
	    data,
	    stats,
	    index,
	    data.getId(channels.at(ChannelId::lightness).dimensions(),
	        options.dimLabelIndex(ChannelId::lightness),
	        index));

	colorBase =
	    channels.at(ChannelId::color).hasMeasure()
	        ? ColorBase(color, lightness)
	        : ColorBase(static_cast<uint32_t>(color), lightness);

	sizeFactor = getValueForChannel(channels,
	    ChannelId::size,
	    data,
	    stats,
	    index,
	    sizeId);

	if (subAxisList != options.subAxis().dimensions())
		subId.label =
		    data.getId(options.subAxis().dimensions(),
		            options.dimLabelIndex(+options.subAxisType()),
		            index)
		        .label;

	auto horizontal = options.isHorizontal();
	auto lineOrCircle = options.geometry == ShapeType::line
	                 || options.geometry == ShapeType::circle;
	auto polar = options.coordSystem.get() == CoordSystem::polar;

	auto xHasMeas = channels.at(AxisId::x).hasMeasure();
	auto yHasMeas = channels.at(AxisId::y).hasMeasure();

	position.x = size.x = getValueForChannel(channels,
	    ChannelId::x,
	    data,
	    stats,
	    index,
	    horizontal ? mainId : subId);

	auto yChannelRectDim = !yHasMeas
	                    && channels.at(AxisId::y).hasDimension()
	                    && options.geometry == ShapeType::rectangle
	                    && channels.axisPropsAt(AxisId::y).align
	                           != Base::Align::Type::stretch;

	spacing.x =
	    (horizontal || (lineOrCircle && !polar) || yChannelRectDim)
	            && options.getChannels().anyAxisSet() && !xHasMeas
	        ? 1
	        : 0;

	position.y = size.y = getValueForChannel(channels,
	    ChannelId::y,
	    data,
	    stats,
	    index,
	    !horizontal ? mainId : subId);

	auto xChannelRectDim = !xHasMeas
	                    && channels.at(AxisId::x).hasDimension()
	                    && options.geometry == ShapeType::rectangle
	                    && channels.axisPropsAt(AxisId::x).align
	                           != Base::Align::Type::stretch;

	spacing.y = (!horizontal || lineOrCircle || xChannelRectDim)
	                 && options.getChannels().anyAxisSet()
	                 && !yHasMeas
	              ? 1
	              : 0;

	if (auto &&labelChannel = channels.at(ChannelId::label);
	    !labelChannel.isEmpty()) {
		auto &&value = std::make_optional(getValueForChannel(channels,
		    ChannelId::label,
		    data,
		    stats,
		    index,
		    data.getId(labelChannel.dimensions(),
		        options.dimLabelIndex(ChannelId::label),
		        index)));

		label =
		    Label{labelChannel.hasMeasure() ? value : std::nullopt,
		        data.joinDimensionValues(labelChannel.dimensions(),
		            index)};
	}

	if (!std::isfinite(position.x) || !std::isfinite(position.y))
		enabled = false;
}

bool Marker::connectMarkers(bool first,
    Marker *prev,
    Marker *next,
    bool main,
    bool polarConnection)
{
	if (prev && next && main && (!first || polarConnection)) {
		next->prevMainMarker =
		    RelativeMarkerIndex{prev->idx, prev - next};
		next->polarConnection = polarConnection && first;
		return true;
	}

	return false;
}

Conv::JSONObj &&Marker::appendToJSON(Conv::JSONObj &&jsonObj) const
{
	return std::move(jsonObj).merge(cellInfo->json);
}

double Marker::getValueForChannel(const Channels &channels,
    ChannelId type,
    const Data::DataCube &data,
    ChannelStats &stats,
    const Data::MultiIndex &index,
    const Data::MarkerId &mid) const
{
	const auto &channel = channels.at(type);

	if (channel.isEmpty()) {
		static constexpr auto defVals =
		    Refl::EnumArray<ChannelId, double>::make(
		        {{ChannelId::color, 0.0},
		            {ChannelId::lightness, 0.5},
		            {ChannelId::size, 0.0},
		            {ChannelId::label, 0.0},
		            {ChannelId::x, 1.0},
		            {ChannelId::y, 1.0},
		            {ChannelId::noop, 0.0}});

		return defVals[type];
	}

	double value{};

	if (const auto &measure = channel.measure()) {
		if (channel.stackable)
			value = data.aggregateAt(index, type, *measure);
		else
			value = data.valueAt(index, *measure);

		if (enabled) stats.track(type, value);
	}
	else {
		if (channel.stackable)
			value = 1.0;
		else
			value = static_cast<double>(mid.itemId);

		if (enabled) stats.track(type, mid);
	}
	return value;
}

Geom::Rect Marker::toRectangle() const
{
	return {position - size, {size}};
}

Math::Range<> Marker::getSizeBy(AxisId axisId) const
{
	return toRectangle().oSize(orientation(axisId));
}

void Marker::setSizeBy(AxisId axisId, const Math::Range<> range)
{
	auto o = orientation(axisId);
	position.getCoord(o) = range.max;
	size.getCoord(o) = range.size();
}

bool Marker::Label::operator==(const Label &other) const
{
	return value == other.value && indexStr == other.indexStr;
}
}
