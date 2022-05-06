#ifndef DATAFILTER_H
#define DATAFILTER_H

#include <functional>

#include "data/table/datatable.h"

namespace Vizzu
{
namespace Data
{

class Filter
{
public:
	typedef std::function<bool(const RowWrapper &)> Function;

	Filter() : function() {}
	template <class Fn>
	Filter(Fn function) : function(function) {}

	bool match(const RowWrapper &row) const {
		return !function || function(row);
	}

	bool operator==(const Filter& other) const {
		return !function && !other.function;
	}

private:
	Function function;
};

}
}

#endif
