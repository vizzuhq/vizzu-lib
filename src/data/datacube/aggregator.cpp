#include "aggregator.h"

#include <algorithm>
#include <limits>
#include <stdexcept>

using namespace Vizzu;
using namespace Vizzu::Data;

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
	case Mean: value = (value * count + v) / (count + 1); break;
	case Exists: value = 1; break;
	case Count: value++; break;
	case Sum: value += v; break;
	case Distinct:
		distinctCategories.insert(v);
		value = distinctCategories.size();
		break;

	default:
		throw std::logic_error(
		    "internal error: invalid sub cell type");
	}

	count++;

	return *this;
}

Aggregator &Aggregator::add(const Aggregator &other)
{
	switch (type) {
	case Min: value = std::min(other.value, value); break;
	case Max: value = std::max(other.value, value); break;
	case Mean:
		value = (value * count + other.value * other.count)
		      / (count + other.count);
		break;
	case Exists: value = std::max(other.value, value); break;
	case Count: value += other.count; break;
	case Sum: value += other.value; break;

	case Distinct:
		distinctCategories.insert(other.distinctCategories.begin(),
		    other.distinctCategories.end());
		value = distinctCategories.size();
		break;

	default:
		throw std::logic_error(
		    "internal error: invalid sub cell type");
	}

	count += other.count;

	return *this;
}

bool Aggregator::isEmpty() const { return count == 0; }

Vizzu::Data::Aggregator::operator double() const { return value; }
