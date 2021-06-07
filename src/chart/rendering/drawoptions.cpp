#include "drawoptions.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

DrawOptions::DrawOptions(bool animation, bool dragdrop)
{
	enableBackground = !dragdrop;
	resoultionMode = animation ? Low : High;
}

