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
#include "chart/options/options.h"
#include "dataframe/old/types.h"

#include "colorbase.h"

namespace Vizzu::Gen
{

struct ChannelStats
{
	using TrackType = std::variant<Math::Range<double>,
	    std::vector<std::optional<Data::SliceIndex>>>;

	Refl::EnumArray<ChannelId, TrackType> tracked;

	template <ChannelIdLike T>
	[[nodiscard]] const TrackType &at(const T &id) const
	{
		return tracked[asChannel(id)];
	}

	void track(ChannelId at, const Data::MarkerId &id)
	{
		auto &vec = std::get<1>(tracked[at]);
		vec[id.itemId] = id.label;
	}

	void track(ChannelId at, const double &value)
	{
		std::get<0>(tracked[at]).include(value);
	}

	void setIfRange(AxisId at, const Math::Range<double> &range)
	{
		if (auto *r = std::get_if<0>(&tracked[asChannel(at)]))
			*r = range;
	}
};

struct MeasureAxis
{
	::Anim::Interpolated<bool> enabled{false};
	Math::Range<double> range = Math::Range<double>::Raw(0, 1);
	::Anim::String unit;
	::Anim::Interpolated<double> step{1.0};
	MeasureAxis() = default;
	MeasureAxis(Math::Range<double> interval,
	    const std::string_view &unit,
	    std::optional<double> step);
	bool operator==(const MeasureAxis &other) const;
	[[nodiscard]] double origo() const;
};

MeasureAxis interpolate(const MeasureAxis &op0,
    const MeasureAxis &op1,
    double factor);

struct DimensionAxis
{
	friend DimensionAxis interpolate(const DimensionAxis &op0,
	    const DimensionAxis &op1,
	    double factor);

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

		Item(Math::Range<double> range, double value) :
		    start(true),
		    end(true),
		    range(range),
		    value(value),
		    weight(1.0)
		{}

		Item(const Item &item, bool starter, double factor) :
		    start(starter),
		    end(!starter),
		    range(item.range),
		    value(item.value),
		    colorBase(item.colorBase),
		    label(item.label),
		    categoryValue(item.categoryValue),
		    weight(Math::FuzzyBool::And(item.weight, factor))
		{}

		bool operator==(const Item &other) const
		{
			return range == other.range && weight == other.weight;
		}

		[[nodiscard]] bool presentAt(
		    ::Anim::InterpolateIndex index) const
		{
			return (index == ::Anim::first && start)
			    || (index == ::Anim::second && end);
		}
	};
	using Values = std::multimap<Data::SliceIndex, Item>;

	bool enabled{false};
	std::string category{};

	DimensionAxis() = default;
	bool add(const Data::SliceIndex &index,
	    double value,
	    const Math::Range<double> &range,
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

struct Axis
{
	::Anim::String title;
	MeasureAxis measure;
	DimensionAxis dimension;

	[[nodiscard]] bool operator==(const Axis &other) const = default;
};

struct Axises
{
	Refl::EnumArray<LegendId, Axis> legend;
	Refl::EnumArray<AxisId, Axis> axises;
	struct Label
	{
		::Anim::String unit;
		::Anim::String origMeasureName;

		[[nodiscard]] bool operator==(
		    const Label &other) const = default;
	} label;

	[[nodiscard]] const Axis &at(LegendId legendType) const
	{
		return legend[legendType];
	}

	[[nodiscard]] Axis &at(LegendId legendType)
	{
		return legend[legendType];
	}

	[[nodiscard]] const Axis &at(AxisId axisType) const
	{
		return axises[axisType];
	}

	[[nodiscard]] Axis &at(AxisId axisType)
	{
		return axises[axisType];
	}

	[[nodiscard]] const Axis &other(AxisId axisType) const
	{
		switch (axisType) {
		default:
		case AxisId::x: return at(AxisId::y);
		case AxisId::y: return at(AxisId::x);
		}
	}

	[[nodiscard]] Geom::Point origo() const;

	[[nodiscard]] bool operator==(
	    const Axises &other) const = default;
};

}

#endif
