#include "columninfo.h"

#include <algorithm>

#include "base/conv/auto_json.h"
#include "base/math/floating.h"
#include "base/text/naturalcmp.h"

namespace Vizzu::Data
{

ColumnInfo::ColumnInfo(std::string name,
    std::string unit,
    TextType textType) :
    name(std::move(name)),
    unit(std::move(unit))
{
	switch (textType) {
	case TextType::Number: contiType = ContiType::Integer; break;

	default: type = Type::dimension; break;
	}
}

std::string ColumnInfo::toJSON() const
{
	std::string res;
	{
		Conv::JSONObj j{res};
		j("name", name)("type", type)("unit", unit)("length", count);

		if (type == Type::measure)
			j("range", range);
		else
			j("categories", values);
	}
	return res;
}

void ColumnInfo::sort()
{
	std::sort(values.begin(), values.end(), Text::NaturalCmp());
	valueIndexes.clear();
	for (auto i = 0U; i < values.size(); ++i)
		valueIndexes.insert({values[i], i});
}

void ColumnInfo::reset()
{
	count = 0;
	if (type == ColumnInfo::Type::measure)
		contiType = ContiType::Integer;
	range = Math::Range<double>();
}

ColumnInfo::Type ColumnInfo::getType() const { return type; }

ColumnInfo::ContiType ColumnInfo::getContiType() const
{
	return contiType;
}

std::span<const std::string> ColumnInfo::categories() const
{
	return values;
}

std::string ColumnInfo::getName() const { return name; }

const std::string &ColumnInfo::getUnit() const { return unit; }

Math::Range<double> ColumnInfo::getRange() const { return range; }

double ColumnInfo::registerValue(double value)
{
	++count;

	switch (type) {
	case Type::measure: {
		range.include(value);
		if (!Math::Floating(value).isInteger())
			contiType = ContiType::Float;
		return value;
	} break;

	case Type::dimension:
		throw std::logic_error(
		    "internal error, double as isDimension value");

	default: throw std::logic_error("internal error, no series type");
	}
}

double ColumnInfo::registerValue(const std::string &value)
{
	++count;

	switch (type) {
	case Type::measure: {
		if (value.empty()) {
			auto val = 0.0;
			range.include(val);
			return val;
		}

		const char *strVal = value.c_str();
		char *eof{};
		auto val = std::strtod(strVal, &eof);
		if (eof == strVal)
			throw std::logic_error(
			    "internal error, cell should be numeric: " + value);

		range.include(val);
		if (!Math::Floating(val).isInteger())
			contiType = ContiType::Float;

		return val;
	} break;

	case Type::dimension: {
		auto it = valueIndexes.find(value);
		if (it != valueIndexes.end()) {
			return static_cast<double>(it->second);
		}

		auto index = values.size();
		values.push_back(value);
		valueIndexes.insert({value, index});
		return static_cast<double>(index);
	} break;

	default:;
	}
	throw std::logic_error("internal error, no series type");
}

std::string ColumnInfo::toString(double value) const
{
	if (type == Type::measure) return std::to_string(value);
	if (type == Type::dimension)
		return values.at(static_cast<size_t>(value));
	return "N.A.";
}

const char *ColumnInfo::toDimensionString(double value) const
{
	if (type == Type::dimension)
		return values.at(static_cast<size_t>(value)).c_str();
	return nullptr;
}

std::string ColumnInfo::toString() const
{
	auto res = name;
	if (type == Type::measure)
		; // res += " (" + std::to_string(count) + ")";
	else
		res += " [" + std::to_string(values.size()) + "]";
	return res;
}

size_t ColumnInfo::minByteWidth() const
{
	if (type == Type::dimension) {
		if (values.size() <= 0x7F) return 1;
		if (values.size() <= 0x7FFF) return 2;
		if (values.size() <= 0x7FFFFFFF) return 4;
		return 8;
	}
	if (type == Type::measure) {
		if (contiType == ContiType::Float) return 8;
		if (contiType == ContiType::Integer) {
			if (range.getMin() >= -1 * 0x7FLL
			    && range.getMax() <= 0x7FLL)
				return 1;
			if (range.getMin() >= -1 * 0x7FFFLL
			    && range.getMax() <= 0x7FFFLL)
				return 2;
			if (range.getMin() >= -1 * 0x7FFFFFFFLL
			    && range.getMax() <= 0x7FFFFFFFLL)
				return 4;
			return 8;
		}
	}
	return 8;
}

}