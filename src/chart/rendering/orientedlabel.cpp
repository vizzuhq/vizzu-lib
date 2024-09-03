#include "orientedlabel.h"

#include <cmath>
#include <memory>
#include <numbers>
#include <optional>
#include <string>
#include <utility>

#include "base/geom/angle.h"
#include "base/geom/line.h"
#include "base/geom/point.h"
#include "base/gfx/canvas.h"
#include "base/gfx/font.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/style.h"

#include "drawlabel.h"

namespace Vizzu::Draw
{

void OrientedLabel::draw(Gfx::ICanvas &canvas,
    const std::string &text,
    const Geom::Line &labelPos,
    const Styles::OrientedLabel &labelStyle,
    double centered,
    const Gfx::ColorTransform &colorTransform,
    Util::EventDispatcher::Event &event,
    std::unique_ptr<Util::EventTarget> eventTarget) const
{
	auto baseAngle =
	    labelPos.getDirection().angle() + std::numbers::pi / 2.0;

	auto absAngle =
	    labelStyle.orientation->combine(
	        [&labelPos](const auto &orientation)
	        {
		        using enum Styles::OrientedLabel::Orientation;
		        switch (orientation) {
		        default:
		        case horizontal: return 0.0;
		        case vertical: return std::numbers::pi / 2.0;
		        case normal: return labelPos.getDirection().angle();
		        case tangential:
			        return labelPos.getDirection().angle()
			             + std::numbers::pi / 2.0;
		        }
	        })
	    + labelStyle.angle->rad();

	auto relAngle = Geom::Angle(absAngle - baseAngle).rad();
	if (relAngle > std::numbers::pi) relAngle -= std::numbers::pi;

	auto xOffsetAngle = std::numbers::pi;
	if (relAngle < std::numbers::pi / 4.0)
		xOffsetAngle = 0.0;
	else if (relAngle < 3.0 * std::numbers::pi / 4.0)
		xOffsetAngle /= 2.0;

	const Gfx::Font font(labelStyle);
	canvas.setFont(font);

	auto paddedSize =
	    labelStyle.extendSize(Gfx::ICanvas::textBoundary(font, text),
	        font.size);

	auto offset = Geom::Point{-sin(relAngle + xOffsetAngle)
	                              * paddedSize.x / 2.0,
	                  -fabs(cos(relAngle)) * paddedSize.y / 2
	                      - sin(relAngle) * paddedSize.x / 2}
	            * (1 - centered) * labelPos.getDirection().abs();

	auto transform =
	    Geom::AffineTransform(labelPos.begin, 1.0, -baseAngle)
	    * Geom::AffineTransform(offset, 1.0, -relAngle)
	    * Geom::AffineTransform(paddedSize / -2, 1.0, 0);

	if (auto realAngle = Geom::Angle(baseAngle + relAngle).rad();
	    realAngle > std::numbers::pi / 2.0
	    && realAngle < 3 * std::numbers::pi / 2.0)
		transform = transform
		          * Geom::AffineTransform(paddedSize,
		              1.0,
		              -std::numbers::pi);

	DrawLabel::draw(canvas,
	    {transform, paddedSize},
	    text,
	    labelStyle,
	    event,
	    std::move(eventTarget),
	    {.colorTransform = colorTransform});
}

}