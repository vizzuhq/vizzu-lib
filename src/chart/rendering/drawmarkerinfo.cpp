#include "drawmarkerinfo.h"

#include "base/io/log.h"
#include "chart/rendering/items/blendeditem.h"
#include "painter/coordinatesystem.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

drawMarkerInfo::drawMarkerInfo(Geom::Rect plot, Gfx::ICanvas &canvas, const Diag::Diagram &diagram) {
	auto coordSys = Draw::CoordinateSystem(
		plot,
		diagram.getOptions()->angle.get(),
		diagram.getOptions()->polar.get(),
		diagram.keepAspectRatio);
	auto& info = diagram.getMarkersInfo();
	auto weight = info.values[0].weight;
	for(auto& item : info.get()) {
		if (item.second != (uint64_t)-1) {
			auto& marker = diagram.getMarkers()[item.first];
			Draw::BlendedDrawItem blendedMarker(
				marker,
				*diagram.getOptions(),
				diagram.getStyle(),
				coordSys,
				diagram.getMarkers(),
				0);
			canvas.setLineWidth(3);
			canvas.setLineColor(Gfx::Color(1, 0, 0));
			auto line = blendedMarker.getLabelPos(
				Styles::MarkerLabel::Position::top,
				coordSys
			);
			Geom::Rect rect(
				line.begin - Geom::Point{10, 10},
				Geom::Point{20, 20}
			);
			canvas.rectangle(rect);
		}
	}
	IO::log() << "mid: " << weight;
}
