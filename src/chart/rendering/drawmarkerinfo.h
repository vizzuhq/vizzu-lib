#ifndef CHART_RENDERING_DRAWMARKERINFO_H
#define CHART_RENDERING_DRAWMARKERINFO_H

#include "base/gfx/canvas.h"
#include "chart/generator/diagram.h"
#include "chart/main/style.h"

namespace Vizzu
{
namespace Draw
{

class drawMarkerInfo
{
public:
	drawMarkerInfo(Geom::Rect plot, Gfx::ICanvas &canvas, const Diag::Diagram &diagram);

private:
};

}
}

#endif //CHART_RENDERING_DRAWMARKERINFO_H
