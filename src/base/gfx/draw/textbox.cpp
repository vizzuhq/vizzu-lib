#include "textbox.h"

#include <cstddef>
#include <utility>

#include "base/geom/point.h"
#include "base/gfx/canvas.h"
#include "base/gfx/color.h"
#include "base/gfx/font.h"

namespace Gfx::Draw
{

const TextBox::Font TextBox::bold{0.0, 1};
const TextBox::Font TextBox::italic{0.0, 2};
const TextBox::Font TextBox::normal{0.0, 3};

TextBox::Padding::Padding() : top(0), left(0), bottom(0), right(0) {}

TextBox::Padding::Padding(double l, double t, double r, double b) :
    top(l),
    left(t),
    bottom(r),
    right(b)
{}

TextBox &TextBox::operator<<(const TabPos &tp)
{
	size.x = size.y = 0;
	tabulators.emplace_back(tp.pos == 0.0, tp.pos);
	return *this;
}

TextBox &TextBox::operator<<(const Gfx::Color &color)
{
	palette.push_back(color);
	return *this;
}

TextBox &TextBox::operator<<(const LineSpacing &ls)
{
	currentLine.spacing = ls.value;
	if (currentLine.spacing < 1) currentLine.spacing = 1;
	if (currentLine.spacing > 3) currentLine.spacing = 3;
	return *this;
}

TextBox &TextBox::operator<<(const Padding &p)
{
	size.x = size.y = 0;
	padding = p;
	return *this;
}

TextBox &TextBox::operator<<(const Geom::Point &p)
{
	position = p;
	return *this;
}

TextBox &TextBox::operator<<(const Gfx::Font &font)
{
	size.x = size.y = 0;
	newTextRun();
	currentTextRun.font = font;
	return *this;
}

TextBox &TextBox::operator<<(const char *str)
{
	size.x = size.y = 0;
	currentTextRun.content += str;
	return *this;
}

TextBox &TextBox::operator<<(const std::string &str)
{
	size.x = size.y = 0;
	currentTextRun.content += str;
	return *this;
}

TextBox &TextBox::operator<<(const Tab &)
{
	currentTextRun.tabulated = true;
	newTextRun();
	return *this;
}

TextBox &TextBox::operator<<(const NewLine &)
{
	newLine();
	return *this;
}

TextBox &TextBox::operator<<(const Font &font)
{
	newTextRun();
	switch (font.opCode) {
	case 0: currentTextRun.font.size = font.size; break;
	case 1:
		currentTextRun.font.weight = Gfx::Font::Weight::Bold();
		break;
	case 2:
		currentTextRun.font.style = Gfx::Font::Style::italic;
		break;
	case 3:
		currentTextRun.font.weight = Gfx::Font::Weight::Normal();
		currentTextRun.font.style = Gfx::Font::Style::normal;
		break;
	default: break;
	}
	return *this;
}

TextBox &TextBox::operator<<(const Bkgnd &color)
{
	if (currentTextRun.backgroundColor.has_value()
	    && color.colorIndex != *currentTextRun.backgroundColor)
		newTextRun();
	currentTextRun.backgroundColor = color.colorIndex;
	return *this;
}

TextBox &TextBox::operator<<(const Fgnd &color)
{
	if (currentTextRun.foregroundColor.has_value()
	    && color.colorIndex != *currentTextRun.foregroundColor)
		newTextRun();
	currentTextRun.foregroundColor = color.colorIndex;
	return *this;
}

void TextBox::draw(ICanvas &canvas, double opacity)
{
	measure(canvas);
	double ypos = position.y + padding.top;
	for (auto &line : lines) {
		double xpos = position.x + padding.left;
		for (auto &text : line.texts) {
			canvas.setFont(text.font);
			Gfx::Color foreground(0, 0, 0, 1);
			Gfx::Color background(1, 1, 1, 1);
			if (text.foregroundColor.has_value()
			    && text.foregroundColor
			           < static_cast<int>(palette.size()))
				foreground = palette[*text.foregroundColor];
			if (text.backgroundColor.has_value()
			    && text.backgroundColor
			           < static_cast<int>(palette.size()))
				background = palette[*text.backgroundColor];
			foreground.alpha *= opacity;
			background.alpha *= opacity;
			canvas.setLineWidth(0);
			canvas.setBrushColor(background);
			canvas.setLineColor(background);
			canvas.rectangle(
			    {{xpos, ypos}, {text.width, line.height}});
			canvas.setBrushColor(foreground);
			canvas.text({{xpos, ypos}, {10000, 10000}}, text.content);
			xpos += text.width;
		}
		ypos += line.height * line.spacing;
	}
}

Geom::Size TextBox::measure(ICanvas &canvas)
{
	if (size.x == 0 || size.y == 0) {
		if (!currentTextRun.content.empty()) newTextRun();
		if (!currentLine.texts.empty()) newLine();
		size.x = size.y = 0;
		for (auto &line : lines) {
			size_t tabPos = 0;
			line.width = 0;
			line.height = 0;
			for (auto &text : line.texts) {
				canvas.setFont(text.font);
				auto size =
				    ICanvas::textBoundary(text.font, text.content);
				text.width = size.x;
				line.width += size.x;
				if (size.y > line.height) line.height = size.y;
				if (text.tabulated) {
					if (tabulators.size() > tabPos
					    && tabulators[tabPos].first
					    && line.width > tabulators[tabPos].second) {
						tabulators[tabPos].second = line.width;
					}
					++tabPos;
				}
			}
		}
		for (auto &line : lines) {
			size_t tabPos = 0;
			line.width = 0;
			for (auto &text : line.texts) {
				if (text.tabulated) {
					if (tabulators.size() > tabPos
					    && tabulators[tabPos].second > line.width)
						text.width += tabulators[tabPos].second
						            - line.width - text.width;
					++tabPos;
				}
				line.width += text.width;
			}
			if (line.width > size.x) size.x = line.width;
			size.y += line.height * line.spacing;
		}
		if (lines.rbegin() != lines.rend()) {
			auto height = lines.rbegin()->height;
			size.y -= height * lines.rbegin()->spacing - height;
		}
		size.x += padding.left + padding.right;
		size.y += padding.top + padding.bottom;
	}
	return size;
}

void TextBox::newTextRun()
{
	size.x = size.y = 0;
	if (!currentTextRun.content.empty()) {
		auto &preTextRun =
		    currentLine.texts.emplace_back(std::move(currentTextRun));
		currentTextRun =
		    TextRun{.foregroundColor = preTextRun.foregroundColor,
		        .backgroundColor = preTextRun.backgroundColor,
		        .font = preTextRun.font};
	}
}

void TextBox::newLine()
{
	newTextRun();
	auto &back = lines.emplace_back(std::move(currentLine));
	currentLine = Line{.spacing = back.spacing};
}

}