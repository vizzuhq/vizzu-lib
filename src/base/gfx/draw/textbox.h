#ifndef GFX_DRAW_TEXTBOX
#define GFX_DRAW_TEXTBOX

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"

namespace Gfx
{
namespace Draw
{

// todo: optimize textbox string and textrun handlling with continous string array and smart pointers
class TextBox
{
public:
    struct Tab {
        static constexpr double Auto = 0.0;
    };

    struct NewLine {
    };

    struct Font {
        Font(double s, bool b, bool i) : size(s), bold(b), italic(i) {}
        Gfx::Font fill(const Gfx::Font& font);
        double size;
        bool bold;
        bool italic;
    };

    static const Font bold;
    static const Font italic;

    struct BgColor {
        BgColor(int i) : colorIndex(i) {}
        int colorIndex;
    };

    struct FgColor {
        FgColor(int i) : colorIndex(i) {}
        int colorIndex;
    };

public:
	TextBox(ICanvas &canvas, const Gfx::Font& defaultFont);

    TextBox& operator<<(const char*);
    TextBox& operator<<(const std::string&);
    TextBox& operator<<(const Tab&);
    TextBox& operator<<(const NewLine&);
    TextBox& operator<<(const Font&);
    TextBox& operator<<(const BgColor&);
    TextBox& operator<<(const FgColor&);

    void clearText();
    Geom::Size size();
    void draw(const Geom::Point& position, double opacity);
    void padding(double l, double r, double t, double b);
    int addColor(const Gfx::Color& color);
    int addTabulator(double width);

private:
    struct TextRun {
        bool tabulated;
        int foregroundColor;
        int backgroundColor;
        double width;
        Font font;
        std::string content;

        TextRun();
    };

    struct Line {
        double height;
        double width;
        std::vector<TextRun> texts;
    };

private:
	ICanvas &canvas;
    Gfx::Font defaultFont;
    Line currentLine;
    TextRun currentTextRun;
    Geom::Size boxSize;
    double left, right, top, bottom;
    std::vector<std::pair<bool, double>> tabulators;
    std::vector<Line> lines;
    std::vector<Gfx::Color> palette;

    void newTextRun();
    void newLine();
    void measureTextExtent();
};

}
}

#endif //GFX_DRAW_TEXTBOX
