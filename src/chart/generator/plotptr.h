#ifndef PLOTPTR_H
#define PLOTPTR_H

#include <memory>

namespace Vizzu::Gen
{
class Plot;
using PlotPtr = std::shared_ptr<Plot>;

class Marker;
}

#endif
