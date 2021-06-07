#ifndef SCALERANGE_H
#define SCALERANGE_H

#include "base/type/physicalvalue.h"
#include "base/math/range.h"

namespace Vizzu
{
namespace Diag
{

class ScaleRange : public Type::PhysicalValue<Math::Range<double>>
{
    typedef Type::PhysicalValue<Math::Range<double>> BaseClass;
public:
    using BaseClass::PhysicalValue;
    using BaseClass::operator=;
    using BaseClass::operator==;

    Math::Range<double> getValue(const Math::Range<double> original)
    {
        if (unit == Type::SimpleUnit::absolute)
            return value;
		else if (unit == Type::SimpleUnit::relative)
			return original.scale(value);
        else return original;
    }
};

}
}

#endif
