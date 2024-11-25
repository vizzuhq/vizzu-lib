#ifndef DRAWAXES_H
#define DRAWAXES_H

#include "base/geom/line.h"
#include "chart/generator/axis.h"
#include "chart/generator/plot.h" // NOLINT(misc-include-cleaner)

#include "drawingcontext.h"

namespace Vizzu::Draw
{

// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
class DrawAxes : public DrawingContext
{
public:
	void drawGeometries() const;
	void drawLabels() const;

	Gfx::ICanvas &canvas;
	Painter &painter;

	[[nodiscard]] const Gen::Axis &getAxis(
	    Gen::AxisId axisIndex) const
	{
		return plot->axises.at(axisIndex);
	}

	[[nodiscard]] Geom::Point origo() const
	{
		return plot->axises.origo();
	}

	struct Separator
	{
		double position{};
		double weight{};
		std::optional<double> label{};
	};

	struct Interval
	{
		struct DimLabel
		{
			const Data::SliceIndex &index;
			const ::Anim::Interpolated<bool> &presented;
			bool start;
			bool end;

			[[nodiscard]] bool presentAt(
			    ::Anim::InterpolateIndex index) const
			{
				return (index == ::Anim::first && start)
				    || (index == ::Anim::second && end);
			}
		};
		Math::Range<> range{};
		double weight{};
		double isSecond{};
		std::optional<DimLabel> label{};
	};

	[[nodiscard]] const DrawAxes &&init() &&;

	Refl::EnumArray<Gen::AxisId, std::vector<Interval>> intervals;
	Refl::EnumArray<Gen::AxisId, std::vector<Separator>> separators;

	[[nodiscard]] const auto &getIntervals(
	    Gen::AxisId axisIndex) const
	{
		return intervals[axisIndex];
	}

	[[nodiscard]] const auto &getSeparators(
	    Gen::AxisId axisIndex) const
	{
		return separators[axisIndex];
	}

private:
	void generateMeasure(Gen::AxisId axisIndex,
	    double stepSize,
	    double weight);
	[[nodiscard]] Geom::Line getAxisLine(Gen::AxisId axisIndex) const;
	[[nodiscard]] Geom::Point getTitleBasePos(Gen::AxisId axisIndex,
	    ::Anim::InterpolateIndex index) const;
	[[nodiscard]] Geom::Point getTitleOffset(Gen::AxisId axisIndex,
	    ::Anim::InterpolateIndex index,
	    bool fades) const;
	void drawAxis(Gen::AxisId axisIndex) const;
	void drawTitle(Gen::AxisId axisIndex) const;
	void drawDimensionLabels(Gen::AxisId axisIndex) const;
	void drawDimensionLabel(Gen::AxisId axisIndex,
	    const Geom::Point &origo,
	    const Interval &interval,
	    double weight) const;
};

}

#endif
