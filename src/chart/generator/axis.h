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
#include "data/datacube/datacube.h"
#include "data/multidim/multidimindex.h"
#include "data/table/datatable.h"

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
		for (auto i = 0; i < std::size(axises); i++) {
			auto id = static_cast<ChannelId>(i);
			if (axises[id] != other.axises[id]) return false;
		}
		return true;
	}
};

struct Axis
{
	::Anim::Interpolated<bool> enabled{false};
	Math::Range<double> range = Math::Range<double>(0, 1);
	::Anim::String title;
	std::string unit;
	::Anim::Interpolated<double> step{1.0};
	Axis() = default;
	Axis(Math::Range<double> interval,
	    std::string title,
	    std::string unit,
	    std::optional<double> step);
	bool operator==(const Axis &other) const;
	[[nodiscard]] double origo() const;
};

Axis interpolate(const Axis &op0, const Axis &op1, double factor);

struct Axises : public AbstractAxises<Axis>
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
		Gfx::Color color;
		std::string label;
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
		    color(item.color),
		    label(item.label),
		    weight(item.weight * factor)
		{}

		bool operator==(const Item &other) const
		{
			return range == other.range;
		}

		[[nodiscard]] bool presentAt(int index) const
		{
			return index == 0 ? start : index == 1 && end;
		}
	};
	using Values = std::map<Data::MultiDim::SliceIndex, Item>;

	Math::FuzzyBool enabled{false};
	::Anim::String title;

	DimensionAxis() = default;
	bool add(const Data::MultiDim::SliceIndex &index,
	    double value,
	    Math::Range<double> &range,
	    double enabled);
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
	void setLabels(const Data::DataCube &data,
	    const Data::DataTable &table);

private:
	Values values;
};

DimensionAxis interpolate(const DimensionAxis &op0,
    const DimensionAxis &op1,
    double factor);

using DimensionAxises = AbstractAxises<DimensionAxis>;

}

#endif
