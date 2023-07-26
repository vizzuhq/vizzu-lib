#ifndef COLUMNINDEX_H
#define COLUMNINDEX_H

#include <cstddef>
#include <cstdint>

#include "base/type/uniquetype.h"

namespace Vizzu
{
namespace Data
{

typedef Type::UniqueType<uint64_t, struct ColumnIndexTypeId>
    ColumnIndex;

}
}

#endif
