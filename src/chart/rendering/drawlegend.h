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
	    Gen::ChannelId channelType,
	    double weight) const;

	const Events::DrawEvents::Legend &events = rootEvents.draw.legend;
	const Styles::Legend &style = rootStyle.legend;

	const ColorBuilder colorBuilder = {
	    rootStyle.plot.marker.lightnessRange(),
	    *rootStyle.plot.marker.colorPalette,
	    *rootStyle.plot.marker.colorGradient};

private:
	struct ColorGradientSetter
	{
		Geom::Line line;
		Gfx::ColorGradient gradient;
		std::span<Gfx::ColorGradient::Stop> modifiableStops =
		    gradient.stops;

		void operator()(Gfx::ICanvas &,
		    const Geom::AffineTransform &,
		    const Gfx::Color &) const;
	};
	struct Info
	{
		Gfx::ICanvas &canvas;
		Geom::Rect titleRect;
		Geom::Rect markerWindowRect;
		double fadeHeight{};
		Gen::ChannelId type{};
		double weight{};
		double itemHeight{};
		double markerSize{};
		const Gen::MeasureAxis &measure;
		const Gen::DimensionAxis &dimension;
		double measureEnabled = measure.enabled.calculate<double>();
		bool dimensionEnabled = dimension.enabled;
		double measureWeight = weight * measureEnabled;
		Events::Targets::LegendProperties properties{};
		ColorGradientSetter colorGradientSetter;
	};

	void drawTitle(const Info &info) const;

	void drawDimension(const Info &info) const;
	void drawMeasure(const Info &info) const;

	void drawMarker(const Info &info,
	    std::string_view categoryValue,
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

	[[nodiscard]] static double markersLegendFullSize(
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
