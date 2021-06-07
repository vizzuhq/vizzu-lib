#ifndef DRAWOPTIONS_H
#define DRAWOPTIONS_H

#include <cstdint>

#include "painter/painteroptions.h"

namespace Vizzu
{
namespace Draw
{

class DrawOptions
{
public:

	DrawOptions(bool animation, bool dragdrop);
	ResolutionMode getResoultionMode() const { return resoultionMode; }
	bool onlyEssentials() const { return !enableBackground; }

private:
	ResolutionMode resoultionMode;
	bool enableBackground;
};

}
}

#endif
