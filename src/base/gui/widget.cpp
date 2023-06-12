#include "widget.h"

#include <optional>

#include "base/util/templates.h"

#include "dragobject.h"

using namespace GUI;

Widget::Widget(const Widget *parent) : parent(parent)
{
	backgroundColor = Gfx::Color::Transparent();
	setMargin({0, 0, 0, 0});
	expand = {false, false};
	enabled = true;
	interactive = true;
	paintable = true;
	eventTransparent = true;
	minSize = Geom::Size(0, 0);
	maxSize = Geom::Size(std::numeric_limits<double>::max(),
	    std::numeric_limits<double>::max());
}

Widget::~Widget()
{
	for (const auto &child : children) child->parent = nullptr;
}

DragObjectPtr Widget::onPointerDown(const GUI::PointerEvent &event)
{
	if (!isEnabled() || !isPaintable()) return DragObjectPtr();

	for (const auto &child : Util::Reverse(children))
		if (child->getBoundary().contains(event.pos)) {
			auto dragObject = child->onPointerDown(event);
			if (dragObject) return dragObject;
		}

	return eventTransparent
	         ? DragObjectPtr()
	         : std::make_shared<DragObject>(shared_from_this());
}

bool Widget::onPointerUp(const GUI::PointerEvent &event,
    DragObjectPtr dragObject)
{
	if (isEnabled() && isPaintable())
		for (const auto &child : Util::Reverse(children))
			if (child->getBoundary().contains(event.pos))
				if (child->onPointerUp(event, dragObject)) {
					dragObject = DragObjectPtr();
					return true;
				}
	return !eventTransparent;
}

bool Widget::onPointerMove(const GUI::PointerEvent &event,
    DragObjectPtr &dragObject)
{
	if (isEnabled() && isPaintable())
		for (const auto &child : Util::Reverse(children))
			if (child->getBoundary().contains(event.pos))
				if (child->onPointerMove(event, dragObject))
					return true;
	return !eventTransparent;
}

bool Widget::onWheel(double delta)
{
	if (isEnabled() && isPaintable())
		for (const auto &child : Util::Reverse(children))
			if (child->onWheel(delta)) { return true; }
	return !eventTransparent;
}

bool Widget::onKeyPress(const Key &key, const KeyModifiers &modifiers)
{
	if (isEnabled() && isPaintable())
		for (const auto &child : Util::Reverse(children))
			if (child->onKeyPress(key, modifiers)) { return true; }
	return !eventTransparent;
}

std::string Widget::getHint(const Geom::Point &pos)
{
	if (isEnabled() && isPaintable())
		for (const auto &child : Util::Reverse(children))
			if (child->getBoundary().contains(pos)) {
				auto hint = child->getHint(pos);
				if (!hint.empty()) return hint;
			}
	return std::string();
}

void Widget::updateSize(Gfx::ICanvas &canvas, Geom::Size &size)
{
	if (isEnabled()) {
		auto neededSize = size;

		onUpdateSize(canvas, neededSize);

		neededSize = Geom::Size::section(
		    Geom::Size::boundary(neededSize, minSize),
		    maxSize);

		size.x =
		    expand.horizontal
		        ? std::min(std::max(size.x, neededSize.x), maxSize.x)
		        : neededSize.x;

		size.y =
		    expand.vertical
		        ? std::min(std::max(size.y, neededSize.y), maxSize.y)
		        : neededSize.y;
	}
	else
		size = Geom::Size(0, 0);
	boundary.size = size;
}

void Widget::draw(Gfx::ICanvas &canvas)
{
	if (isEnabled() && isPaintable()
	    && canvas.getClipRect().intersects(boundary)) {
		if (backgroundColor.alpha > 0) {
			canvas.setLineColor(backgroundColor);
			canvas.setBrushColor(backgroundColor);
			canvas.rectangle(boundary);
		}
		onDraw(canvas);
	}
}

void Widget::dragLeft(const DragObjectPtr &dragObject)
{
	dragObject->updateOvered(std::weak_ptr<Widget>());
}

void Widget::onChanged() const
{
	if (parent) parent->onChanged();
}

void Widget::onUpdateSize(Gfx::ICanvas &, Geom::Size &size)
{
	boundary.size = size;
}

Geom::AffineTransform Widget::getSelfTransform() const
{
	return Geom::AffineTransform();
}

Geom::AffineTransform Widget::getTransform() const
{
	Geom::AffineTransform res;
	if (parent) res = parent->getTransform();
	res = res * getSelfTransform();
	return res;
}

void Widget::onDraw(Gfx::ICanvas &canvas)
{
	for (const auto &child : children) child->draw(canvas);
}

void Widget::setPos(const Geom::Point &pos)
{
	auto delta = pos - boundary.pos;
	boundary.pos = boundary.pos + delta;
	for (const auto &child : children)
		child->setPos(child->getBoundary().pos + delta);
}

Geom::Point Widget::getPos() const { return boundary.pos; }

void Widget::setMargin(const Margin &value) { margin = value; }

void Widget::setExpand(const Directions &enable) { expand = enable; }

void Widget::setEnabled(bool enable) { enabled = enable; }

void Widget::setMinSize(const Geom::Size &size) { minSize = size; }

void Widget::setMaxSize(const Geom::Size &size) { maxSize = size; }

void Widget::setName(const std::string &name)
{
	this->objName = name;
}

void Widget::setEventTransparent(bool transparent)
{
	this->eventTransparent = transparent;
}

std::string Widget::getName() const { return objName; }

std::string Widget::getQualifiedName() const
{
	std::string res;
	if (parent) res += parent->getQualifiedName() + "/";
	res += getName();
	return res;
}

Margin Widget::getMargin() const { return margin; }

Directions Widget::isExpand() const { return expand; }

bool Widget::isEnabled() const { return enabled; }

bool Widget::isPaintable() const { return paintable; }

bool Widget::isInteractive() const { return interactive; }

Gfx::Color Widget::getBgColor() const { return backgroundColor; }

void Widget::setBgColor(const Gfx::Color &color)
{
	backgroundColor = color;
}

Geom::Rect Widget::getBoundary() const { return boundary; }

Geom::Rect Widget::getContentRect() const
{
	return Geom::Rect(boundary.pos + margin.topLeft(),
	    boundary.size - margin.getSpace());
}

void Widget::clear() { children.clear(); }

const Widget *Widget::getParent() const { return parent; }

bool Widget::isChild(const std::shared_ptr<Widget> &widget)
{
	for (const auto &child : children) {
		if (widget == child || child->isChild(widget)) return true;
	}
	return false;
}

void Widget::removeChild(const std::shared_ptr<Widget> &widget)
{
	if (widget) {
		auto it = std::find(children.begin(), children.end(), widget);
		if (it != children.end()) children.erase(it);
	}
}

void Widget::moveToTop(const std::weak_ptr<Widget> &widget)
{
	auto w = widget.lock();
	if (w) {
		auto it = std::find(children.begin(), children.end(), w);
		if (it != children.end()) {
			children.erase(it);
			children.push_back(w);
		}
	}
}

void Widget::updateBoundary() { boundary = childrenBoundary(); }

Geom::Rect Widget::childrenBoundary() const
{
	if (children.empty()) { return Geom::Rect(); }
	else {
		std::optional<Geom::Rect> res;
		for (const auto &child : children)
			if (child->isEnabled()) {
				if (res)
					*res = res->boundary(child->getBoundary());
				else
					*res = child->getBoundary();
			}
		return *res;
	}
}

std::weak_ptr<Widget> Widget::widgetAt(const Geom::Point &pos) const
{
	for (const auto &child : children)
		if (child->getBoundary().contains(pos)) return child;
	return std::weak_ptr<Widget>();
}

const Widget *Widget::findRoot() const
{
	if (parent)
		return parent->findRoot();
	else
		return this;
}

void Widget::setCursor(Cursor cursor) const
{
	if (parent) return parent->setCursor(cursor);
}

bool Widget::swapChild(const std::shared_ptr<Widget> &toRemove,
    const std::shared_ptr<Widget> &toAdd)
{
	auto it = std::find(children.begin(), children.end(), toRemove);
	if (it != children.end()) {
		toAdd->parent = this;
		*it = toAdd;
		return true;
	}
	else
		return false;
}

void ContainerWidget::moveAfter(const std::shared_ptr<Widget> &child,
    const std::shared_ptr<Widget> &posChild)
{
	auto it = std::find(children.begin(), children.end(), child);
	auto posIt =
	    std::find(children.begin(), children.end(), posChild);

	if (it != children.end() && posIt != children.end()) {
		children.erase(it);
		++posIt;
		children.insert(posIt, child);
	}
}
