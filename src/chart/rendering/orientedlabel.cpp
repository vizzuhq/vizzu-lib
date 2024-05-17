#include "orientedlabel.h"

#include "base/geom/angle.h"

namespace Vizzu::Draw
{

void OrientedLabel::draw(Gfx::ICanvas &canvas,
    const std::string &text,
    const Geom::Line &labelPos,
    const Styles::OrientedLabel &labelStyle,
    double centered,
    std::optional<double> textAlpha,
    double bgAlpha,
    Util::EventDispatcher::Event &event,
    std::unique_ptr<Util::EventTarget> eventTarget) const
{
	auto baseAngle = labelPos.getDirection().angle() + M_PI / 2.0;

	auto absAngle =
	    labelStyle.orientation->combine<double>(
	        [&labelPos](const auto &orientation)
	        {
		        using enum Styles::OrientedLabel::Orientation;
		        switch (orientation) {
		        default:
		        case horizontal: return 0.0;
		        case vertical: return M_PI / 2.0;
		        case normal: return labelPos.getDirection().angle();
		        case tangential:
			        return labelPos.getDirection().angle()
			             + M_PI / 2.0;
		        }
	        })
	    + labelStyle.angle->rad();

	auto relAngle = Geom::Angle(absAngle - baseAngle).rad();
	if (relAngle > M_PI) relAngle -= M_PI;

	auto xOffsetAngle = relAngle < M_PI / 4.0     ? 0
	                  : relAngle < 3 * M_PI / 4.0 ? M_PI / 2.0
	                                              : M_PI;

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
	    realAngle > M_PI / 2.0 && realAngle < 3 * M_PI / 2.0)
		transform =
		    transform * Geom::AffineTransform(paddedSize, 1.0, -M_PI);

	DrawLabel::draw(canvas,
	    {transform, paddedSize},
	    text,
	    labelStyle,
	    event,
	    std::move(eventTarget),
	    {.alpha = textAlpha, .bgAlpha = bgAlpha});
}

}