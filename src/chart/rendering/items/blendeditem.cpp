#include "blendeditem.h"

#include "base/math/arrayoperators.h"

#include "areaitem.h"
#include "circleitem.h"
#include "lineitem.h"
#include "rectangleitem.h"

using namespace Vizzu;
using namespace Math;
using namespace Vizzu::Draw;

BlendedDrawItem::BlendedDrawItem(const Gen::Marker &marker,
    const Gen::Options &options,
    const Styles::Chart &style,
    const CoordinateSystem &coordSys,
    const Gen::Plot::Markers &markers,
    size_t lineIndex) :
    DrawItem(marker)
{
	std::array<DrawItem, 4> drawItems = {
	    RectangleItem(marker, options, style),
	    CircleItem(marker, options, style, coordSys),
	    LineItem(marker,
	        options,
	        style,
	        coordSys,
	        markers,
	        lineIndex),
	    AreaItem(marker, options, markers, lineIndex)};

	enabled = false;
	labelEnabled = false;
	for (auto &item : drawItems) {
		enabled += item.enabled;
		labelEnabled += item.labelEnabled;
	}

	auto lineEnabled = drawItems[2].enabled;
	drawItems[2].enabled = false;
	blend(drawItems, &DrawItem::morphToCircle, &DrawItem::enabled);
	drawItems[2].enabled = lineEnabled;

	blend(drawItems, &DrawItem::linear, &DrawItem::enabled);
	blend(drawItems, &DrawItem::border, &DrawItem::enabled);
	blend(drawItems, &DrawItem::points, &DrawItem::enabled);
	blend(drawItems, &DrawItem::lineWidth, &DrawItem::enabled);
	blend(drawItems, &DrawItem::connected, &DrawItem::enabled);
	blend(drawItems, &DrawItem::color, &DrawItem::enabled);
	blend(drawItems, &DrawItem::center, &DrawItem::enabled);
	blend(drawItems, &DrawItem::dataRect, &DrawItem::labelEnabled);
	blend(drawItems, &DrawItem::radius, &DrawItem::labelEnabled);
}

template <typename T, size_t N>
void BlendedDrawItem::blend(std::array<DrawItem, N> &items,
    T DrawItem::*member,
    Math::FuzzyBool(DrawItem::*enable))
{
	this->*member = T();
	double cnt = 0;
	for (auto &item : items)
		if ((double)(item.*enable) > 0) {
			this->*member =
			    this->*member + item.*member * (double)(item.*enable);
			cnt += (double)(item.*enable);
		}
	if (cnt != 0)
		this->*member = this->*member * (1.0 / cnt);
	else
		this->*member = T();
}
