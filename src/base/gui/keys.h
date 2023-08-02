#ifndef GUI_KEYS
#define GUI_KEYS

#include <cstdint>

namespace GUI
{

enum class Key : uint16_t {
	pageup = 256,
	pagedown,
	home,
	end,
	left,
	right,
	up,
	down,
	escape,
	tab,
	enter,
	del
};

struct KeyModifiers
{
	bool shift;
	bool ctrl;
	bool alt;
	KeyModifiers() : shift(false), ctrl(false), alt(false) {}
	KeyModifiers(bool shift, bool ctrl, bool alt) :
	    shift(shift),
	    ctrl(ctrl),
	    alt(alt)
	{}
	[[nodiscard]] bool none() const
	{
		return !shift && !ctrl && !alt;
	}
	[[nodiscard]] bool onlyShift() const
	{
		return shift && !ctrl && !alt;
	}
	[[nodiscard]] bool onlyCtrl() const
	{
		return !shift && ctrl && !alt;
	}
	[[nodiscard]] bool onlyAlt() const
	{
		return !shift && !ctrl && alt;
	}
};

}

#endif
