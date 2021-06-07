#include "label.h"

using namespace GUI;

Label::Label(std::string text, const Widget *parent)
	: Widget(parent),
	  text(std::move(text))
{
	expand = { true, false };
	angle = 0;
	setTextStyle(Gfx::Color::Black(), 14);
	setBgColor(Gfx::Color::Transparent());
}

void Label::onUpdateSize(Gfx::ICanvas &canvas, Geom::Size &size)
{
	canvas.setFont(Gfx::Font(textSize));

	clippedText = getText();

	Geom::Size neededSize;

	if (!clippedText.empty())
		neededSize = canvas.textBoundary(clippedText, angle);

	neededSize = neededSize + margin.getSpace();

	if (!maxSize.bounds(neededSize) && clippedText.size() > 4)
	{
		clippedText = clippedText + "...";

		while(!maxSize.bounds(neededSize) && clippedText.size() > 4)
		{
			clippedText = clippedText.substr(0, clippedText.size() - 4) + "...";
			neededSize = canvas.textBoundary(clippedText, angle)
					+ margin.getSpace();
		}
	}

	size = neededSize;
}

void Label::onDraw(Gfx::ICanvas &canvas)
{
	canvas.setFont(Gfx::Font(textSize));
	canvas.setTextColor(textColor);
	canvas.text(Geom::Rect(boundary.pos + margin.topLeft(),
						   boundary.size - margin.getSpace()),
				clippedText, angle);
}

void Label::setText(const std::string &text)
{
	this->text = text;
}

void Label::setTextColor(const Gfx::Color &color)
{
	textColor = color;
}

void Label::setTextSize(double size)
{
	textSize = size;
}

void Label::setTextStyle(const Gfx::Color &color, double size)
{
	setTextColor(color);
	setTextSize(size);
}

void Label::setAngle(double value)
{
	angle = value;
}

std::string Label::getText() const
{
	return text;
}

Gfx::Color Label::getTextColor() const
{
	return textColor;
}

double Label::getTextSize() const
{
	return textSize;
}
