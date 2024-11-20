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
	using TrackType = std::variant<Math::Range<>,
	    std::vector<std::optional<Data::SliceIndex>>>;

	Refl::EnumArray<ChannelId, TrackType> tracked;
	Math::Range<> lightness;

	template <ChannelIdLike T>
	[[nodiscard]] const TrackType &at(const T &id) const
	{
		return tracked[-id];
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
	void setIfRange(Id at, const Math::Range<> &range)
	{
		if (auto *r = std::get_if<0>(&tracked[-at])) *r = range;
	}
};

struct MeasureAxis
{
	::Anim::Interpolated<bool> enabled{false};
	Math::Range<> range = Math::Range<>::Raw(0, 1);
	std::string series;
	::Anim::String unit;
	::Anim::Interpolated<double> step{1.0};

	MeasureAxis() = default;
	MeasureAxis(const Math::Range<> &interval,
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
		explicit Item() = default;

	public:
		bool start{};
		bool end{};
		Math::Range<> range;
		::Anim::Interpolated<std::uint32_t> position;
		::Anim::Interpolated<ColorBase> colorBase;
		::Anim::Interpolated<bool> label;

		Item(Math::Range<> range,
		    const std::optional<std::uint32_t> &position,
		    const std::optional<ColorBase> &color,
		    bool setCategoryAsLabel) :
		    start(true),
		    end(true),
		    range(range),
		    label(setCategoryAsLabel)
		{
			if (position) this->position = *position;
			if (color) colorBase = *color;
		}

		Item(const Item &item, bool starter) :
		    start(starter),
		    end(!starter),
		    range(item.range),
		    position(item.position),
		    colorBase(item.colorBase),
		    label(item.label)
		{}

		bool operator==(const Item &other) const
		{
			return range == other.range && position == other.position;
		}

		[[nodiscard]] bool presentAt(
		    ::Anim::InterpolateIndex index) const
		{
			return (index == ::Anim::first && start)
			    || (index == ::Anim::second && end);
		}

		[[nodiscard]] double weight(double atEnd) const
		{
			return Math::Niebloid::interpolate(start, end, atEnd);
		}

		friend Item
		interpolate(const Item &op0, const Item &op1, double factor);
	};
	using Values = std::multimap<Data::SliceIndex, Item>;

	double factor{};

	DimensionAxis() = default;
	bool add(const Data::SliceIndex &index,
	    const Math::Range<> &range,
	    const std::optional<std::uint32_t> &position,
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

	[[nodiscard]] const std::string &seriesName() const
	{
		if (!dimension.empty())
			return dimension.getValues().begin()->first.column;
		return measure.series;
	}

	[[nodiscard]] bool operator==(const Axis &other) const = default;
};

struct Axises
{
	struct CalculatedLegend
	{
		LegendId type;
		Axis calc;
		std::uint64_t interpolated{};
	};
	std::array<std::optional<CalculatedLegend>, 2> leftLegend;

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

	[[nodiscard]] Geom::Point origo() const;

private:
	[[nodiscard]] static const Axis &empty();
};

}

#endif
