#ifndef AXIS_H
#define AXIS_H

#include <map>
#include <ranges>

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
	    std::map<std::uint32_t, std::vector<Data::SliceIndex>>>;

	Refl::EnumArray<ChannelId, TrackType> tracked;
	Math::Range<> lightness;

	template <ChannelIdLike T>
	[[nodiscard]] const TrackType &at(const T &id) const
	{
		return tracked[+id];
	}

	void track(ChannelId at, const Data::MarkerId &id)
	{
		auto &vec = std::get<1>(tracked[at]);
		if (!id.label.empty())
			vec.try_emplace(static_cast<std::uint32_t>(id.itemId),
			    id.label);
	}

	void track(ChannelId at, const double &value)
	{
		if (std::isfinite(value))
			std::get<0>(tracked[at]).include(value);
	}

	template <ChannelIdLike Id>
	void setIfRange(Id at, const Math::Range<> &range)
	{
		if (auto *r = std::get_if<0>(&tracked[+at])) *r = range;
	}
};

struct MeasureAxis
{
	::Anim::Interpolated<bool> enabled{false};
	Math::Range<> range{0, 1};
	std::string series;
	::Anim::String unit;
	::Anim::Interpolated<double> step{1.0};

	MeasureAxis() = default;
	MeasureAxis(const Math::Range<> &interval,
	    std::string &&series,
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
		using PosType = Base::AutoParam<std::uint32_t>;
		PosType startPos{};
		PosType endPos{};
		Math::Range<> range;
		::Anim::Interpolated<ColorBase> colorBase;
		::Anim::Interpolated<bool> label;

		Item(Math::Range<> range,
		    std::uint32_t position,
		    const std::optional<ColorBase> &color,
		    bool setCategoryAsLabel) :
		    startPos(position),
		    endPos(position),
		    range(range),
		    label(setCategoryAsLabel)
		{
			if (color) colorBase = *color;
		}

		Item(const Item &item, bool starter) :
		    startPos(starter ? item.startPos : PosType{}),
		    endPos(starter ? PosType{} : item.endPos),
		    range(item.range),
		    colorBase(item.colorBase),
		    label(item.label)
		{}

		bool operator==(const Item &other) const
		{
			return range == other.range && startPos == other.startPos;
		}

		[[nodiscard]] double weight(double atEnd) const
		{
			return Math::Niebloid::interpolate(!startPos.isAuto(),
			    !endPos.isAuto(),
			    atEnd);
		}

		friend Item
		interpolate(const Item &op0, const Item &op1, double factor);
	};
	using Values = std::multimap<std::vector<Data::SliceIndex>, Item>;

	double factor{};
	bool hasMarker{};

	DimensionAxis() = default;
	bool add(const std::vector<Data::SliceIndex> &index,
	    const Math::Range<> &range,
	    std::uint32_t position,
	    const std::optional<ColorBase> &color,
	    bool label,
	    bool merge);
	[[nodiscard]] bool operator==(
	    const DimensionAxis &other) const = default;

	[[nodiscard]] auto begin()
	{
		return std::views::values(values).begin();
	};
	[[nodiscard]] auto end()
	{
		return std::views::values(values).end();
	}
	[[nodiscard]] auto begin() const
	{
		return std::views::values(values).begin();
	};
	[[nodiscard]] auto end() const
	{
		return std::views::values(values).end();
	}
	[[nodiscard]] bool empty() const { return values.empty(); }
	bool setLabels(double step);

	[[nodiscard]] const Values &getValues() const { return values; }

	[[nodiscard]] auto sortedItems()
	{
		struct ItemSorterByRangeStart
		{
			[[nodiscard]] bool operator()(const Item &lhs,
			    const Item &rhs) const
			{
				return Math::Floating::less(lhs.range.min,
				    rhs.range.min);
			}
		};
		return std::multiset<std::reference_wrapper<Item>,
		    ItemSorterByRangeStart>{begin(), end()};
	}

	static std::size_t commonDimensionParts(const Values &lhs,
	    const Values &rhs);

	template <std::string Data::SliceIndex::*which =
	              &Data::SliceIndex::value>
	[[nodiscard]] static std::string mergedLabels(
	    const std::vector<Data::SliceIndex> &slices)
	{
		return {std::from_range,
		    std::views::transform(slices,
		        [](const Data::SliceIndex &slice)
		        {
			        return ", " + slice.*which;
		        })
		        | std::views::join | std::views::drop(2)};
	}

private:
	Values values;
};

struct Axis
{
	::Anim::String title;
	MeasureAxis measure;
	DimensionAxis dimension;

	[[nodiscard]] std::string seriesName() const
	{
		if (!dimension.empty()) {
			return DimensionAxis::mergedLabels<
			    &Data::SliceIndex::column>(
			    dimension.getValues().begin()->first);
		}
		return measure.series;
	}

	[[nodiscard]] bool operator==(const Axis &other) const = default;
};

struct SplitAxis : Axis
{
	struct Part
	{
		double weight{1.0};
		Math::Range<> range{0, 1};
		Math::Range<> measureRange{0, 1};
		bool unique{};

		[[nodiscard]] bool operator==(
		    const Part &other) const = default;

		[[nodiscard]] consteval static auto members()
		{
			return std::tuple{&Part::weight,
			    &Part::range,
			    &Part::measureRange,
			    std::ignore};
		}
	};

	using Parts = std::multimap<std::vector<Data::SliceIndex>, Part>;
	Parts parts;

	[[nodiscard]] bool operator==(
	    const SplitAxis &other) const = default;

	friend SplitAxis interpolate(const SplitAxis &op0,
	    const SplitAxis &op1,
	    double factor);
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

	Refl::EnumArray<AxisId, SplitAxis> axises;
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

	[[nodiscard]] const SplitAxis &at(AxisId axisType) const
	{
		return axises[axisType];
	}

	[[nodiscard]] SplitAxis &at(AxisId axisType)
	{
		return axises[axisType];
	}

	[[nodiscard]] Geom::Point origo() const;

private:
	[[nodiscard]] static const Axis &empty();
};

}

#endif
