#include "listwidget.h"

#include <algorithm>

using namespace GUI;

ListWidget::ListWidget(bool horizontal, const Widget *parent) :
    ContainerWidget(parent),
    horizontal(horizontal)
{
	align = Align::Min;
	setCellSpacing(0);
}

void ListWidget::setCellSpacing(double value) { cellSpacing = value; }

void ListWidget::setAlign(Align value) { align = value; }

void ListWidget::onUpdateSize(Gfx::ICanvas &canvas, Geom::Size &size)
{
	Geom::Size max(0, 0);
	subCoord(max) = subCoord(size) - subCoord(margin.getSpace());

	double expandLength = 0.0;

	for (const auto &child : children) {
		Geom::Size min;
		child->updateSize(canvas, min);
		subCoord(max) = std::max(subCoord(min), subCoord(max));
		mainCoord(max) += mainCoord(min);
		if (child->isExpand().getMain(horizontal)) {
			mainCoord(min) = std::max(1.0, mainCoord(min));
			expandLength += mainCoord(min);
		}
	}

	auto pos = margin.topLeft();

	double expandSpace = mainCoord(size) - mainCoord(max)
	                   - mainCoord(margin.getSpace());

	for (const auto &child : children) {
		child->setPos(boundary.pos + pos);
		Geom::Size min(0, 0);
		subCoord(min) = subCoord(max);
		child->updateSize(canvas, min);

		if (child->isExpand().getMain(horizontal)
		    && expandSpace > 0) {
			mainCoord(min) = std::max(1.0, mainCoord(min));
			mainCoord(min) =
			    mainCoord(min) * expandSpace / expandLength;
			child->updateSize(canvas, min);
		}

		mainCoord(pos) += mainCoord(min) + cellSpacing;

		switch (align) {
		case Align::Min: break;

		case Align::Center: {
			auto rect = child->getBoundary();
			subCoord(rect.pos) +=
			    (subCoord(max) - subCoord(rect.size)) / 2;
			child->setPos(rect.pos);
		} break;

		case Align::Max: {
			auto rect = child->getBoundary();
			subCoord(rect.pos) += subCoord(max) - subCoord(rect.size);
			child->setPos(rect.pos);
		} break;
		}
	}

	mainCoord(pos) += -cellSpacing + mainCoord(margin.bottomRight());
	subCoord(boundary.size) =
	    subCoord(max) + subCoord(margin.getSpace());
	mainCoord(boundary.size) = mainCoord(pos);
	size = boundary.size;
}

bool ListWidget::isHorizontal() const { return horizontal; }

void ListWidget::setHorizontal(bool horizontal)
{
	this->horizontal = horizontal;
}

double ListWidget::subCoord(const Geom::Point &point) const
{
	return horizontal ? point.y : point.x;
}

double ListWidget::mainCoord(const Geom::Point &point) const
{
	return horizontal ? point.x : point.y;
}

double &ListWidget::subCoord(Geom::Point &point)
{
	return horizontal ? point.y : point.x;
}

double &ListWidget::mainCoord(Geom::Point &point)
{
	return horizontal ? point.x : point.y;
}
