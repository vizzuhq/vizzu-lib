#include "aggregator.h"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace Vizzu::Data
{

Aggregator::Aggregator(Type type) : type(type)
{
	switch (type) {
	case Min: value = std::numeric_limits<double>::max(); break;
	case Max: value = std::numeric_limits<double>::lowest(); break;
	case Mean:
	case Exists:
	case Count:
	case Sum:
	case Distinct: value = 0; break;

	default:
		throw std::logic_error(
		    "internal error: invalid sub cell type");
	}
}

Aggregator &Aggregator::add(double v)
{
	switch (type) {
	case Min: value = std::min(v, value); break;
	case Max: value = std::max(v, value); break;
	case Mean:
		value = (value * static_cast<double>(count) + v)
		      / static_cast<double>(count + 1);
		break;
	case Exists: value = 1; break;
	case Count: ++value; break;
	case Sum: value += v; break;
	case Distinct:
		distinctCategories.insert(static_cast<int>(v));
		value = static_cast<double>(distinctCategories.size());
		break;

	default:
		throw std::logic_error(
		    "internal error: invalid sub cell type");
	}

	++count;

	return *this;
}

Aggregator &Aggregator::add(const Aggregator &other)
{
	switch (type) {
	case Min: value = std::min(other.value, value); break;
	case Max: value = std::max(other.value, value); break;
	case Mean:
		value = (value * static_cast<double>(count)
		            + other.value * static_cast<double>(other.count))
		      / static_cast<double>(count + other.count);
		break;
	case Exists: value = std::max(other.value, value); break;
	case Count: value += static_cast<double>(other.count); break;
	case Sum: value += other.value; break;

	case Distinct:
		distinctCategories.insert(other.distinctCategories.begin(),
		    other.distinctCategories.end());
		value = static_cast<double>(distinctCategories.size());
		break;

	default:
		throw std::logic_error(
		    "internal error: invalid sub cell type");
	}

	count += other.count;

	return *this;
}

bool Aggregator::isEmpty() const { return count == 0; }

Vizzu::Data::Aggregator::operator double() const
{
	return isEmpty() ? 0.0 : value;
}

}