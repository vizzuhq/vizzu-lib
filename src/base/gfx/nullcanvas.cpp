#include "nullcanvas.h"

using namespace Gfx;

ICanvasPtr NullCanvas::createCanvas(int, int)
{
    return std::make_shared<NullCanvas>();
}
