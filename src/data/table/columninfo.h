#ifndef SERIESINFO_H
#define SERIESINFO_H

#include <map>
#include <vector>
#include <string>

#include "base/math/range.h"

namespace Vizzu
{
namespace Data
{

enum TextType : uint32_t { Number, String };

class ColumnInfo
{
public:
	enum Type : uint32_t { Continous = 0, Discrete = 1 };
	enum class ContiType : uint8_t { Unknown = 0, Integer = 1, Float = 2 };

	typedef std::map<std::string, uint64_t> ValueIndexes;
	typedef std::vector<std::string> Values;

	ColumnInfo();
	ColumnInfo(const std::string &name, TextType textType);
	void sort();
	void reset();
	Type getType() const;
	ContiType getContiType() const;
	const ValueIndexes &discreteValueIndexes() const;
	const Values &discreteValues() const;
	size_t discreteValueCnt() const;

	std::string getName() const;
	std::string getUnit() const;
	Math::Range<double> getRange() const;
	std::string toString() const;
	size_t minByteWidth() const;

	double registerValue(const std::string &value);
	double registerValue(double value);
	std::string toString(double value) const;
	const char *toDiscreteString(double value) const;

	std::string toJSon() const;

private:
	uint64_t count;
	std::string name;
	std::string unit;
	Type type;
	ContiType contiType;
	Math::Range<double> range;
	ValueIndexes valueIndexes;
	Values values;
};

}
}

#endif
