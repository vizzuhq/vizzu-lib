#ifndef TESTSTYLE_H
#define TESTSTYLE_H

#include <optional>

struct Foo
{
	std::optional<double> foo;
	std::optional<double> bar;

	void visit(auto &visitor) { visitor(foo, "foo")(bar, "bar"); }
};

struct Baz
{
	std::optional<double> baz;
	std::optional<double> fobar;

	void visit(auto &visitor) { visitor(baz, "baz")(fobar, "fobar"); }
};

struct Fobar
{
	Foo foo;
	Baz baz;

	void visit(auto &visitor) { visitor(foo, "foo")(baz, "baz"); }
};

#endif
