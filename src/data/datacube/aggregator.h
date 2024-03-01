#ifndef DATA_DATACUBE_SUBCELL_H
#define DATA_DATACUBE_SUBCELL_H

#include <cstddef>
#include <cstdint>
#include <unordered_set>

namespace Vizzu::Data
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
	[[nodiscard]] explicit operator double() const;
	[[nodiscard]] bool isEmpty() const;

private:
	Type type;
	double value;
	uint64_t count{};
	std::unordered_set<int> distinctCategories;
};

}

#endif
