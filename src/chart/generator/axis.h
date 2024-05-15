#ifndef AXIS_H
#define AXIS_H

#include <map>

#include "base/anim/interpolated.h"
#include "base/geom/point.h"
#include "base/gfx/color.h"
#include "base/math/fuzzybool.h"
#include "base/math/interpolation.h"
#include "base/math/range.h"
#include "chart/options/channel.h"
#include "dataframe/old/types.h"

#include "colorbase.h"

namespace Vizzu::Gen
{

template <typename Type> struct AbstractAxises
{
	Refl::EnumArray<ChannelId, Type> axises;

	[[nodiscard]] const Type &at(ChannelId channelType) const
	{
		return axises.at(channelType);
	}

	Type &at(ChannelId channelType) { return axises.at(channelType); }

	[[nodiscard]] const Type &other(ChannelId channelType) const
	{
		return channelType == ChannelId::x ? axises.at(ChannelId::y)
		     : channelType == ChannelId::y
		         ? axises.at(ChannelId::x)
		         : throw std::logic_error("not an axis channel");
	}

	bool operator==(const AbstractAxises<Type> &other) const
	{
		for (auto i = 0; i < std::size(axises); ++i) {
			auto id = static_cast<ChannelId>(i);
			if (axises[id] != other.axises[id]) return false;
		}
		return true;
	}
};

struct CommonAxis
{
	::Anim::String title;

	[[nodiscard]] bool operator==(const CommonAxis &) const = default;
};

using CommonAxises = AbstractAxises<CommonAxis>;

CommonAxis interpolate(const CommonAxis &op0,
    const CommonAxis &op1,
    double factor);

struct MeasureAxis
{
	::Anim::Interpolated<bool> enabled{false};
	Math::Range<double> range = Math::Range<double>::Raw(0, 1);
	::Anim::String unit;
	::Anim::String origMeasureName;
	::Anim::Interpolated<double> step{1.0};
	MeasureAxis() = default;
	MeasureAxis(Math::Range<double> interval,
	    const std::string_view &unit,
	    const std::string_view &measName,
	    std::optional<double> step);
	bool operator==(const MeasureAxis &other) const;
	[[nodiscard]] double origo() const;
};

MeasureAxis interpolate(const MeasureAxis &op0,
    const MeasureAxis &op1,
    double factor);

struct MeasureAxises : public AbstractAxises<MeasureAxis>
{
	[[nodiscard]] Geom::Point origo() const;
};

struct DimensionAxis
{
	friend DimensionAxis interpolate(const DimensionAxis &op0,
	    const DimensionAxis &op1,
	    double factor);

public:
	class Item
	{
	public:
		bool start;
		bool end;
		Math::Range<double> range;
		double value;
		::Anim::Interpolated<ColorBase> colorBase;
		::Anim::String label;
		std::string categoryValue;
		double weight;

		Item(Math::Range<double> range,
		    double value,
		    double enabled) :
		    start(true),
		    end(true),
		    range(range),
		    value(value),
		    weight(enabled)
		{}

		Item(const Item &item, bool starter, double factor) :
		    start(starter),
		    end(!starter),
		    range(item.range),
		    value(item.value),
		    colorBase(item.colorBase),
		    label(item.label),
		    categoryValue(item.categoryValue),
		    weight(item.weight * factor)
		{}

		bool operator==(const Item &other) const
		{
			return range == other.range;
		}

		[[nodiscard]] bool presentAt(
		    ::Anim::InterpolateIndex index) const
		{
			return (index == ::Anim::first && start)
			    || (index == ::Anim::secondIfExists && end);
		}
	};
	using Values = std::multimap<Data::SliceIndex, Item>;

	bool enabled{false};
	std::string category{};

	DimensionAxis() = default;
	bool add(const Data::SliceIndex &index,
	    double value,
	    const Math::Range<double> &range,
	    double enabled,
	    bool merge);
	bool operator==(const DimensionAxis &other) const;

	Values::iterator begin() { return values.begin(); };
	Values::iterator end() { return values.end(); }
	[[nodiscard]] Values::const_iterator begin() const
	{
		return values.cbegin();
	};
	[[nodiscard]] Values::const_iterator end() const
	{
		return values.cend();
	}
	bool setLabels(double step);

private:
	Values values;
};

DimensionAxis interpolate(const DimensionAxis &op0,
    const DimensionAxis &op1,
    double factor);

using DimensionAxises = AbstractAxises<DimensionAxis>;

}

#endif
