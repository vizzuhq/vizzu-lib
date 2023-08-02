#ifndef GFX_DRAW_TEXTBOX
#define GFX_DRAW_TEXTBOX

#include <optional>

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"

namespace Gfx::Draw
{

class TextBox
{
public:
	struct Padding
	{
		double top;
		double left;
		double bottom;
		double right;

		Padding();
		Padding(double l, double t, double r, double b);
	};

	struct TabPos
	{
		double pos;

		TabPos(double pos) : pos(pos) {}
	};

	struct Font
	{
		double size;
		int opCode;

		Font(double s, int opCode = 0) : size(s), opCode(opCode) {}
	};

	static const Font bold;
	static const Font italic;
	static const Font normal;

	struct Bkgnd
	{
		int colorIndex;

		Bkgnd(int i) : colorIndex(i) {}
	};

	struct Fgnd
	{
		int colorIndex;

		Fgnd(int i) : colorIndex(i) {}
	};

	struct Tab
	{};

	struct NewLine
	{};

	struct LineSpacing
	{
		double value;

		LineSpacing(double v) : value(v) {}
	};

	TextBox &operator<<(const Padding &);
	TextBox &operator<<(const Geom::Point &);
	TextBox &operator<<(const Gfx::Font &);
	TextBox &operator<<(const TabPos &);
	TextBox &operator<<(const Gfx::Color &);
	TextBox &operator<<(const LineSpacing &);

	TextBox &operator<<(const char *);
	TextBox &operator<<(const std::string &);
	TextBox &operator<<(const Tab &);
	TextBox &operator<<(const NewLine &);
	TextBox &operator<<(const Font &);
	TextBox &operator<<(const Bkgnd &);
	TextBox &operator<<(const Fgnd &);

	Geom::Size measure(ICanvas &canvas);
	void draw(ICanvas &canvas, double opacity);

private:
	struct TextRun
	{
		bool tabulated;
		std::optional<int> foregroundColor;
		std::optional<int> backgroundColor;
		double width;
		Gfx::Font font;
		std::string content;

		TextRun();
	};

	struct Line
	{
		double spacing;
		double height;
		double width;
		std::vector<TextRun> texts;

		Line();
	};

	Geom::Size size;
	Geom::Point position;
	Padding padding;
	std::vector<Gfx::Color> palette;
	std::vector<std::pair<bool, double>> tabulators;
	TextRun currentTextRun;
	Line currentLine;
	std::vector<Line> lines;

	void newTextRun();
	void newLine();
};

}

#endif
