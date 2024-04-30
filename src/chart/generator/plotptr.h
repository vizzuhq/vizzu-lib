#ifndef PLOTPTR_H
#define PLOTPTR_H

#include <memory>
#include <vector>

namespace Vizzu::Gen
{
class Plot;
using PlotPtr = std::shared_ptr<Plot>;

class Marker;
using Buckets = std::vector<std::vector<Marker *>>;
}

#endif
