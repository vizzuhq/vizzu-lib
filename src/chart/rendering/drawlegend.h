#ifndef CHART_RENDERING_DRAWLEGEND_H
#define CHART_RENDERING_DRAWLEGEND_H

#include "chart/generator/axis.h"
#include "chart/rendering/colorbuilder.h"
#include "chart/rendering/drawingcontext.h"

namespace Vizzu::Draw
{

class DrawLegend : public DrawingContext
{
public:
	void draw(Gfx::ICanvas &canvas,
	    const Geom::Rect &legendLayout,
	    Gen::LegendId channelType,
	    double weight,
	    const Gen::Axis &axis) const;

	const Events::DrawEvents::Legend &events = rootEvents.draw.legend;
	const Styles::Legend &style = rootStyle.legend;

	const ColorBuilder colorBuilder = {
	    rootStyle.plot.marker.lightnessRange(),
	    *rootStyle.plot.marker.colorPalette,
	    *rootStyle.plot.marker.colorGradient};

private:
	struct FadeBarGradient
	{
		Geom::Line origLine;
		Gfx::LinearGradient gradient;

		[[nodiscard]] const Gfx::LinearGradient &operator()(
		    const Gfx::Color &,
		    const Geom::AffineTransform & = {});
	};
	struct Info
	{
		Gfx::ICanvas &canvas;
		Geom::Rect titleRect;
		Geom::Rect markerWindowRect;
		double fadeHeight{};
		double weight{};
		double itemHeight{};
		double markerSize{};
		const Gen::Axis &axis;
		double measureEnabled =
		    axis.measure.enabled.combine<double>();
		double measureWeight =
		    Math::FuzzyBool::And(weight, measureEnabled);
		Events::Targets::LegendProperties properties;
		FadeBarGradient fadeBarGradient;
	};

	void drawTitle(const Info &info) const;

	void drawDimension(Info &info) const;
	void drawMeasure(const Info &info) const;

	void drawMarker(Info &info,
	    const std::vector<Data::SliceIndex> &sindex,
	    const Gfx::Color &color,
	    const Geom::Rect &rect,
	    bool needGradient) const;
	[[nodiscard]] static Geom::Rect getItemRect(const Info &info,
	    double index);
	[[nodiscard]] static Geom::Rect getMarkerRect(const Info &info,
	    const Geom::Rect &itemRect);
	[[nodiscard]] static Geom::TransformedRect
	getLabelRect(const Info &info, const Geom::Rect &itemRect);
	[[nodiscard]] static Geom::Rect getBarRect(const Info &info);

	[[nodiscard]] static Math::Range<> markersLegendRange(
	    const Info &info);

	void extremaLabel(const Info &info,
	    double value,
	    const std::string &unit,
	    int pos,
	    double plusWeight) const;
	void colorBar(const Info &info, const Geom::Rect &rect) const;
	void lightnessBar(const Info &info, const Geom::Rect &rect) const;
	void sizeBar(const Info &info, const Geom::Rect &rect) const;
};

}

#endif
