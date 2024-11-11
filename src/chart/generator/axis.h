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
	Math::Range<double> lightness;

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

	template <ChannelIdLike Id>
	void setIfRange(Id at, const Math::Range<double> &range)
	{
		if (auto *r = std::get_if<0>(&tracked[asChannel(at)]))
			*r = range;
	}
};

struct MeasureAxis
{
	::Anim::Interpolated<bool> enabled{false};
	Math::Range<double> range = Math::Range<double>::Raw(0, 1);
	std::string series;
	::Anim::String unit;
	::Anim::Interpolated<double> step{1.0};

	MeasureAxis() = default;
	MeasureAxis(const Math::Range<double> &interval,
	    std::string series,
	    const std::string_view &unit,
	    const std::optional<double> &step);
	[[nodiscard]] bool operator==(
	    const MeasureAxis &other) const = default;
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
		::Anim::Interpolated<bool> label;
		double weight;

		Item(Math::Range<double> range,
		    double value,
		    const std::optional<ColorBase> &color,
		    bool setCatAsLabel) :
		    start(true),
		    end(true),
		    range(range),
		    value(value),
		    label(setCatAsLabel),
		    weight(1.0)
		{
			if (color) colorBase = *color;
		}

		Item(const Item &item, bool starter, double factor) :
		    start(starter),
		    end(!starter),
		    range(item.range),
		    value(item.value),
		    colorBase(item.colorBase),
		    label(item.label),
		    weight(item.weight * factor)
		{}

		bool operator==(const Item &other) const
		{
			return range == other.range && weight == other.weight
			    && value == other.value;
		}

		[[nodiscard]] bool presentAt(
		    ::Anim::InterpolateIndex index) const
		{
			return (index == ::Anim::first && start)
			    || (index == ::Anim::second && end);
		}
	};
	using Values = std::multimap<Data::SliceIndex, Item>;

	DimensionAxis() = default;
	bool add(const Data::SliceIndex &index,
	    const Math::Range<double> &range,
	    double value,
	    const std::optional<ColorBase> &color,
	    bool label,
	    bool merge);
	[[nodiscard]] bool operator==(
	    const DimensionAxis &other) const = default;

	[[nodiscard]] auto begin()
	{
		return std::ranges::views::values(values).begin();
	};
	[[nodiscard]] auto end()
	{
		return std::ranges::views::values(values).end();
	}
	[[nodiscard]] auto begin() const
	{
		return std::ranges::views::values(values).begin();
	};
	[[nodiscard]] auto end() const
	{
		return std::ranges::views::values(values).end();
	}
	[[nodiscard]] bool empty() const { return values.empty(); }
	bool setLabels(double step);

	[[nodiscard]] const Values &getValues() const { return values; }

private:
	Values values;
};

struct Axis
{
	::Anim::String title;
	MeasureAxis measure;
	DimensionAxis dimension;

	[[nodiscard]] bool operator==(const Axis &other) const = default;
};

struct Axises
{
	struct CalcLegend
	{
		LegendId type;
		Axis calc;
		std::uint64_t interpolated{};
	};
	std::array<std::optional<CalcLegend>, 2> leftLegend;

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
		if (const auto &l0 = leftLegend[0];
		    l0 && l0->type == legendType)
			return l0->calc;

		return empty();
	}

	void addLegendInterpolation(double legendFactor,
	    LegendId legendType,
	    const Axis &source,
	    const Axis &target,
	    double factor);

	Axis &create(LegendId legendType)
	{
		return leftLegend[0].emplace(legendType).calc;
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

private:
	[[nodiscard]] static const Axis &empty();
};

}

#endif
