#include "textbox.h"

using namespace Gfx::Draw;

const TextBox::Font TextBox::bold{0, true, false};
const TextBox::Font TextBox::italic{0, false, true};

TextBox::TextRun::TextRun()
    : font(10, false, false)
{
    width = 0;
    tabulated = false;
    backgroundColor = -1;
    foregroundColor = -1;
}

Gfx::Font TextBox::Font::fill(const Gfx::Font& font) {
    Gfx::Font result;
    result.family = font.family;
    result.size = size;
    result.style = font.style;
    result.weight = font.weight;
    if (bold)
        result.weight = Gfx::Font::Weight::Bold();
    if (italic)
        result.style = Gfx::Font::Style::oblique;
    return result;
}

TextBox::TextBox(ICanvas &canvas, const Gfx::Font& defaultFont) :
    canvas(canvas), defaultFont(defaultFont),
    left(0), right(0), top(0), bottom(0)
{
    currentLine.width = 0;
    currentLine.height = 0;
}

TextBox& TextBox::operator<<(const char* str) {
    boxSize.x = boxSize.y = 0;
    currentTextRun.content += str;
    return *this;
}

TextBox& TextBox::operator<<(const std::string& str) {
    boxSize.x = boxSize.y = 0;
    currentTextRun.content += str;
    return *this;
}

TextBox& TextBox::operator<<(const Tab&) {
    currentTextRun.tabulated = true;
    newTextRun();
    return *this;
}

TextBox& TextBox::operator<<(const NewLine&) {
    newLine();
    return *this;
}

TextBox& TextBox::operator<<(const Font& font) {
    auto size = currentTextRun.font.size;
    newTextRun();
    currentTextRun.font = font;
    if (font.size == 0)
        currentTextRun.font.size = size;
    return *this;
}

TextBox& TextBox::operator<<(const BgColor& color) {
    if (currentTextRun.backgroundColor != -1 && color.colorIndex != currentTextRun.backgroundColor)
        newTextRun();
    currentTextRun.backgroundColor = color.colorIndex;
    return *this;
}

TextBox& TextBox::operator<<(const FgColor& color) {
    if (currentTextRun.foregroundColor != -1 && color.colorIndex != currentTextRun.foregroundColor)
        newTextRun();
    currentTextRun.foregroundColor = color.colorIndex;
    return *this;
}

void TextBox::clearText() {
    boxSize.x = boxSize.y = 0;
    lines.clear();
    currentLine.texts.clear();
    currentLine.width = 0;
    currentLine.height = 0;
    currentTextRun = TextRun{};
    for(auto& tab : tabulators) {
        if (tab.first)
            tab.second = 0.0;
    }
}

void TextBox::draw(const Geom::Point& position, double opacity) {
    size();
    double ypos = position.y + top;
    for(auto& line : lines) {
        double xpos = position.x + left;
        for(auto& text : line.texts) {
            canvas.setFont(text.font.fill(defaultFont));
            Gfx::Color foreground(0, 0, 0);
            Gfx::Color background(1, 1, 1);
            if (text.foregroundColor >= 0 && text.foregroundColor < (int)palette.size())
                foreground = palette[text.foregroundColor];
            if (text.backgroundColor >= 0 && text.backgroundColor < (int)palette.size())
                background = palette[text.backgroundColor];
            foreground.alpha = opacity;
            background.alpha = opacity;
            canvas.setLineWidth(0);
            canvas.setBrushColor(background);
            canvas.setLineColor(background);
            Geom::Rect bkgnd(xpos, ypos, text.width, line.height);
            canvas.rectangle(bkgnd);
            canvas.setTextColor(foreground);
            canvas.text(Geom::Rect(xpos, ypos, 10000, 10000), text.content.c_str());
            xpos += text.width;
        }
        ypos += line.height;
    }
}

Geom::Size TextBox::size() {
    if (boxSize.x == 0 || boxSize.y == 0) {
        if (!currentTextRun.content.empty())
            newTextRun();
        if (currentLine.texts.size())
            newLine();
        measureTextExtent();
    }
    return boxSize;
}

void TextBox::padding(double l, double r, double t, double b) {
    boxSize.x = boxSize.y = 0;
    left = l, right = r, top = t, bottom = b;
}

int TextBox::addColor(const Gfx::Color& color) {
    palette.push_back(color);
    return palette.size();
}

int TextBox::addTabulator(double width) {
    boxSize.x = boxSize.y = 0;
    tabulators.push_back(std::make_pair(width == 0.0, width));
    return tabulators.size();
}

void TextBox::newTextRun() {
    boxSize.x = boxSize.y = 0;
    if (!currentTextRun.content.empty()) {
        currentLine.texts.push_back(currentTextRun);
        currentTextRun = TextRun{};
        currentTextRun.backgroundColor = currentLine.texts.rbegin()->backgroundColor;
        currentTextRun.foregroundColor = currentLine.texts.rbegin()->foregroundColor;
        currentTextRun.font = currentLine.texts.rbegin()->font;
    }
}

void TextBox::newLine() {
    newTextRun();
    lines.push_back(currentLine);
    currentLine = Line{};
}

void TextBox::measureTextExtent() {
    boxSize.x = boxSize.y = 0;
    for(auto& line : lines) {
        size_t tabPos = 0;
        line.width = 0;
        line.height = 0;
        for(auto& text : line.texts) {
            canvas.setFont(text.font.fill(defaultFont));
            auto size = canvas.textBoundary(text.content);
            text.width = size.x;
            line.width += size.x;
            if (size.y > line.height)
                line.height = size.y;
            if (text.tabulated) {
                if (tabulators.size() > tabPos && tabulators[tabPos].first &&
                    line.width > tabulators[tabPos].second)
                {
                    tabulators[tabPos].second = line.width;
                }
                tabPos++;
            }
        }
    }
    for(auto& line : lines) {
        size_t tabPos = 0;
        line.width = 0;
        for(auto& text : line.texts) {
            if (text.tabulated) {
                if (tabulators.size() > tabPos && tabulators[tabPos].second > line.width)
                    text.width += tabulators[tabPos].second - line.width - text.width;
                tabPos++;
            }
            line.width += text.width;
        }
        if (line.width > boxSize.x)
            boxSize.x = line.width;
        boxSize.y += line.height;
    }
    boxSize.x += left + right;
    boxSize.y += top + bottom;
}
