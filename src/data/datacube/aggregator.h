#ifndef SUBCELL_H
#define SUBCELL_H

#include <cstddef>
#include <cstdint>
#include <unordered_set>

namespace Vizzu
{
namespace Data
{

class Aggregator
{
public:
	enum Type : uint32_t {
		Exists,
		Sum,
		Count,
		Distinct,
		Min,
		Max,
		Mean
	};

	explicit Aggregator(Type type);
	Aggregator &add(double);
	Aggregator &add(const Aggregator &);
	explicit operator double() const;
	bool isEmpty() const;

private:
	Type type;
	double value;
	uint64_t count;
	std::unordered_set<int> distinctCategories;
};

}
}

#endif
