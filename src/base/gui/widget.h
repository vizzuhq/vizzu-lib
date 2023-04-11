#ifndef GUI_WIDGET
#define GUI_WIDGET

#include <memory>
#include <functional>
#include <iterator>
#include <list>
#include <string>

#include "accessories.h"
#include "base/geom/affinetransform.h"
#include "base/geom/rect.h"
#include "base/gfx/canvas.h"
#include "keys.h"
#include "pointer.h"

namespace GUI
{

class DragObject;
typedef std::shared_ptr<DragObject> DragObjectPtr;

class Widget : public std::enable_shared_from_this<Widget>
{
public:

	Widget(const Widget *parent);

	virtual ~Widget();

	virtual DragObjectPtr onPointerDown(const PointerEvent &event);
	virtual bool onPointerUp(const PointerEvent &event, DragObjectPtr dragObject);
	virtual bool onPointerMove(const PointerEvent &event, DragObjectPtr &dragObject);
	virtual bool onWheel(double delta);
	virtual bool onKeyPress(const Key &key, const KeyModifiers &modifiers);	
	virtual void dragLeft(const DragObjectPtr &dragObject);
	virtual void onChanged() const;
	virtual std::string getHint(const Geom::Point &pos);

	void updateSize(Gfx::ICanvas &canvas, Geom::Size &size);
	void draw(Gfx::ICanvas&canvas);
	virtual void setPos(const Geom::Point &pos);
	Geom::Point getPos() const;
	void setMargin(const Margin &value);
	void setExpand(const Directions &enable);
	void setEnabled(bool enable);
	void setMinSize(const Geom::Size &size);
	void setMaxSize(const Geom::Size &size);
	void setName(const std::string &name);
	void setEventTransparent(bool transparent);
	Directions isExpand() const;
	virtual bool isEnabled() const;
	virtual bool isPaintable() const;
	virtual bool isInteractive() const;
	Gfx::Color getBgColor() const;
	void setBgColor(const Gfx::Color &color);
	Geom::Rect getBoundary() const;
	Geom::Rect getContentRect() const;
	Geom::AffineTransform getTransform() const;
	std::string getName() const;
	std::string getQualifiedName() const;
	Margin getMargin() const;
	virtual void clear();
	const Widget *getParent() const;

protected:

	template<typename W>
	std::shared_ptr<W> getAs(const W* = nullptr) {
		auto res = std::dynamic_pointer_cast<W>(shared_from_this());
		if (!res) throw std::logic_error("internal error: invalid widget conversion");
		return res;
	}

	template<typename W, typename... T>
	std::weak_ptr<W> emplaceChild(T... params) {
		children.push_back(std::make_shared<W>(params..., this));
		return std::dynamic_pointer_cast<W>(children.back());
	}

	template<typename W, typename... T>
	std::weak_ptr<W> emplaceChildAt(size_t position, T... params) {
		auto pos = children.begin();
		if (position < children.size())
			std::advance(pos, position);
		else pos = children.end();
		auto it = children.insert(pos, std::make_shared<W>(params..., this));
		return std::dynamic_pointer_cast<W>(*it);
	}


	template <typename W = Widget>
	void visitChildren(std::function<void(const std::shared_ptr<W>&)> visit, bool recursive)
	{
		for (auto child: children) {
			auto w = std::dynamic_pointer_cast<W>(child);
			if (w) visit(w);
			if (recursive) child->visitChildren<W>(visit, recursive);
		}
	}

	const Widget *parent;
	std::string objName;
	bool enabled;
	bool interactive;
	bool paintable;
	bool eventTransparent;
	std::list<std::shared_ptr<Widget>> children;
	Geom::Rect boundary;
	Geom::Size minSize;
	Geom::Size maxSize;
	Margin margin;
	Directions expand;
	Gfx::Color backgroundColor;

	virtual void setCursor(Cursor cursor) const;
	virtual void onDraw(Gfx::ICanvas&);
	virtual void onUpdateSize(Gfx::ICanvas&, Geom::Size &);
	virtual Geom::AffineTransform getSelfTransform() const;
	bool isChild(const std::shared_ptr<Widget> &widget);
	void removeChild(const std::shared_ptr<Widget> &widget);
	bool swapChild(const std::shared_ptr<Widget> &toRemove,
	    const std::shared_ptr<Widget> &toAdd);
	void moveToTop(const std::weak_ptr<Widget> &widget);
	void updateBoundary();
	Geom::Rect childrenBoundary() const;
	std::weak_ptr<Widget> widgetAt(const Geom::Point &pos) const;
	const Widget *findRoot() const;
};

class ContainerWidget : public Widget
{
public:
	using Widget::Widget;
	using Widget::emplaceChild;
	using Widget::emplaceChildAt;
	using Widget::removeChild;
	using Widget::swapChild;
	using Widget::visitChildren;

	void moveAfter(const std::shared_ptr<Widget> &child,
	    const std::shared_ptr<Widget> &posChild);
};

class WrapperWidget : public Widget
{
public:
	using Widget::Widget;

	template<typename W, typename... T>
	std::weak_ptr<W> emplaceOnlyChild(T... params) {
		clear();
		onlyChild = this->emplaceChild<W>(params...);
		return std::dynamic_pointer_cast<W>(onlyChild.lock());
	}

protected:
	std::weak_ptr<Widget> onlyChild;
};

}

#endif
