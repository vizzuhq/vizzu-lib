#ifndef COLUMNINDEX_H
#define COLUMNINDEX_H

#include <cstddef>
#include <cstdint>

#include "base/type/uniquetype.h"

namespace Vizzu::Data
{

using ColumnIndex =
    Type::UniqueType<uint64_t, struct ColumnIndexTypeId>;

}

#endif
