#ifndef SERIESINFO_H
#define SERIESINFO_H

#include <map>
#include <span>
#include <string>
#include <vector>

#include "base/math/range.h"

namespace Vizzu::Data
{

enum class TextType : uint32_t { Number, String };

class ColumnInfo
{
public:
	enum class Type { measure, dimension };

	enum class ContiType : uint8_t {
		Unknown = 0,
		Integer = 1,
		Float = 2
	};

	using ValueIndexes = std::map<std::string, uint64_t>;
	using Values = std::vector<std::string>;

	ColumnInfo() = default;
	ColumnInfo(std::string name, std::string unit, TextType textType);
	void sort();
	void reset();
	[[nodiscard]] Type getType() const;
	[[nodiscard]] ContiType getContiType() const;
	[[nodiscard]] std::span<const std::string> categories() const;

	[[nodiscard]] std::string getName() const;
	[[nodiscard]] const std::string &getUnit() const;
	[[nodiscard]] Math::Range<double> getRange() const;
	[[nodiscard]] std::string toString() const;
	[[nodiscard]] size_t minByteWidth() const;

	double registerValue(const std::string &value);
	double registerValue(double value);
	[[nodiscard]] std::string toString(double value) const;
	[[nodiscard]] const char *toDimensionString(double value) const;

	[[nodiscard]] std::string toJSON() const;

private:
	uint64_t count{};
	std::string name = "undefined";
	std::string unit;
	Type type{Type::measure};
	ContiType contiType{ContiType::Unknown};
	Math::Range<double> range;
	ValueIndexes valueIndexes;
	Values values;
};

}

#endif
