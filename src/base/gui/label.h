#ifndef GUI_LABEL
#define GUI_LABEL

#include <string>

#include "base/gfx/color.h"
#include "base/util/callcache.h"

#include "widget.h"

namespace GUI
{

class Label : public Widget
{
public:
	Label(std::string text, const Widget *parent);
	void onUpdateSize(Gfx::ICanvas &, Geom::Size &) override;
	void onDraw(Gfx::ICanvas &canvas) override;
	void setText(const std::string &text);
	void setTextColor(const Gfx::Color &color);
	void setTextSize(double size);
	void setTextStyle(const Gfx::Color &color, double size);
	virtual std::string getText() const;
	Gfx::Color getTextColor() const;
	double getTextSize() const;

protected:
	Gfx::Color textColor;
	double textSize;
	std::string text;
	std::string clippedText;
};

class Separator : public Label
{
public:
	Separator(const Widget *parent) : Label("", parent) {}
	Separator(Geom::Size size, const Widget *parent) :
	    Label("", parent)
	{
		resize(size);
	}
	void resize(Geom::Size size)
	{
		setMargin({size.y, size.x, 0.0, 0.0});
	}
};

}

#endif
