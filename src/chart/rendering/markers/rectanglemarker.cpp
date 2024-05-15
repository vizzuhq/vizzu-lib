#include "rectanglemarker.h"

namespace Vizzu::Draw
{

RectangleMarker::RectangleMarker(const Gen::Marker &marker,
    const Gen::Options &options,
    const Styles::Chart &style) :
    SingleDrawMarker(marker, options, Gen::ShapeType::rectangle)
{
	linear =
	    options.coordSystem.factor<double>(Gen::CoordSystem::polar)
	    == 0;
	border = Math::FuzzyBool(true);

	auto spacing = Geom::Size{
	    marker.spacing
	    * style.plot.marker.rectangleSpacing->combine<Geom::Size>(
	        [&marker](const auto &rectangleSpacing)
	        {
		        if (rectangleSpacing) {
			        auto padding = *rectangleSpacing;
			        auto spacing = padding / (2 * (padding + 1));
			        return Geom::Size{
			            marker.size * Geom::Size::Square(spacing)};
		        }

		        auto minWidth = 0.2;
		        auto decrease = 5.0;
		        auto minPadding = 0.15;

		        auto spacing =
		            Geom::Point{marker.size.x
		                            - std::max(0.0,
		                                minWidth
		                                    * (1
		                                        - exp(-marker.size.x
		                                              * decrease))),
		                marker.size.y
		                    - std::max(0.0,
		                        minWidth
		                            * (1
		                                - exp(-marker.size.y
		                                      * decrease)))}
		            / 2.0;

		        if (spacing.x < marker.size.x * minPadding)
			        spacing.x = marker.size.x * minPadding;
		        if (spacing.y < marker.size.y * minPadding)
			        spacing.y = marker.size.y * minPadding;

		        return Geom::Size{spacing};
	        })};

	center = marker.position - marker.spacing * marker.size / 2;

	points[0] = marker.position - marker.size + spacing;
	points[1] =
	    marker.position - marker.size.yComp() + spacing.flipX();
	points[2] = marker.position - spacing;
	points[3] =
	    marker.position - marker.size.xComp() + spacing.flipY();
	lineWidth[0] = lineWidth[1] = 0;

	dataRect.pos = points[0];
	dataRect.size = Geom::Size{points[2] - points[0]};
	radius = 0;
}

}