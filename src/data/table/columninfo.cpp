#include "columninfo.h"

#include <algorithm>

#include "base/math/floating.h"
#include "base/text/naturalcmp.h"
#include "base/text/smartstring.h"

#include "texttype.h"

using namespace Vizzu;
using namespace Data;

ColumnInfo::ColumnInfo()
{
	count = 0;
	name = "undefined";
	type = Continous;
	contiType = ContiType::Unknown;
}

ColumnInfo::ColumnInfo(const std::string &name, const TextType &textType)
{
	count = 0;
	contiType = ContiType::Unknown;
	this->name = name;

	auto open = name.find("[");
	auto close = name.find("]");
	auto beg = open + 1;
	auto end = close - 1;
	if(open != std::string::npos
	   && close != std::string::npos
	   && end >= beg)
	{
		unit = name.substr(beg, end - beg + 1);
	};

	switch(textType.getType())
	{
	case TextType::Number:
		type = Continous;
		contiType = ContiType::Integer;
		break;

	default:
		type = Discrete;
		break;
	}
}

void ColumnInfo::sort()
{
	std::sort(values.begin(), values.end(), Text::NaturalCmp());
	valueIndexes.clear();
	for (auto i = 0u; i < values.size(); i++)
		valueIndexes.insert({ values[i], i });
}

ColumnInfo::Type ColumnInfo::getType() const
{
	return type;
}

ColumnInfo::ContiType ColumnInfo::getContiType() const
{
	return contiType;
}

const ColumnInfo::ValueIndexes &ColumnInfo::discreteValueIndexes() const
{
	return valueIndexes;
}

const ColumnInfo::Values &ColumnInfo::discreteValues() const
{
	return values;
}

size_t ColumnInfo::discreteValueCnt() const
{
	return values.size();
}

std::string ColumnInfo::getName() const
{
	return name;
}

std::string ColumnInfo::getUnit() const
{
	return unit;
}

Math::Range<double> ColumnInfo::getRange() const { return range; }

double ColumnInfo::registerValue(double value)
{
	count++;

	switch (type)
	{
	case Continous:
	{
		range.include(value);
		if (!Math::Floating(value).isInteger())
			contiType = ContiType::Float;
		return value;
	} break;

	case Discrete:
		throw std::logic_error("internal error, double as discrete value");

	default:
		throw std::logic_error("internal error, no series type");
	}
}

double ColumnInfo::registerValue(const std::string &value)
{
	count++;

	switch(type)
	{
	case Continous:
	{
		if (value.empty()) {
			double val = 0.0;
			range.include(val);
			return val;
		}

		try
		{
			double val = std::stod(value);
			range.include(val);
			if (!Math::Floating(val).isInteger())
				contiType = ContiType::Float;

			return val;
		} catch(...) {
			throw std::logic_error("internal error, cell should be numeric: "
									+ value);
		}
	} break;

	case Discrete: {
		auto it = valueIndexes.find(value);
		if (it != valueIndexes.end())
		{
			return (double)it->second;
		}
		else
		{
			auto index = values.size();
			values.push_back(value);
			valueIndexes.insert({ value, index });
			return (double)index;
		}
	} break;

	default: ;
	}
	throw std::logic_error("internal error, no series type");
}

std::string ColumnInfo::valueAsString(double value) const
{
	if (type == Continous) return Text::SmartString::fromNumber(value);
	if (type == Discrete) return values.at(value);
	return "N.A.";
}

std::string ColumnInfo::toString(double value) const
{
	if (type == Continous) return std::to_string(value);
	if (type == Discrete) return values.at(value);
	return "N.A.";
}

const char *ColumnInfo::toDiscreteString(double value) const
{
	if (type == Discrete) return values.at(value).c_str();
	return nullptr;
}

std::string ColumnInfo::toString() const
{
	auto res = name;
	if (type == Continous) ;//res += " (" + std::to_string(count) + ")";
	else res += " [" + std::to_string(values.size()) + "]";
	return res;
}

size_t ColumnInfo::minByteWidth() const
{
	if (type == Discrete)
	{
		if (values.size() <= 0x7F) return 1;
		if (values.size() <= 0x7FFF) return 2;
		if (values.size() <= 0x7FFFFFFF) return 4;
		return 8;
	}
	if (type == Continous)
	{
		if (contiType == ContiType::Float) return 8;
		if (contiType == ContiType::Integer) {
			if (range.getMin() >= -1*0x7Fll && range.getMax() <= 0x7Fll) return 1;
			if (range.getMin() >= -1*0x7FFFll && range.getMax() <= 0x7FFFll) return 2;
			if (range.getMin() >= -1*0x7FFFFFFFll && range.getMax() <= 0x7FFFFFFFll) return 4;
			return 8;
		}
	}
	return 8;
}

ColumnInfoConvert::ColumnInfoConvert(const ColumnInfo &from, const ColumnInfo &to)
{
	auto fromIndexes = from.discreteValueIndexes();
	auto toIndexes = to.discreteValueIndexes();
	targetIndexes.resize(fromIndexes.size());
	for (auto &pair : fromIndexes) {
		targetIndexes[pair.second] = toIndexes[pair.first];
	}
}

size_t ColumnInfoConvert::convertValueIndex(size_t from)
{
	return targetIndexes[from];
}
