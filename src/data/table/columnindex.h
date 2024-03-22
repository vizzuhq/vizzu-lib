#ifndef COLUMNINDEX_H
#define COLUMNINDEX_H

#include <cstddef>
#include <cstdint>
#include <functional>

#include "base/type/uniquetype.h"

namespace Vizzu::Data
{

using ColumnIndex =
    Type::UniqueType<uint64_t, struct ColumnIndexTypeId>;

}

template <> struct std::hash<Vizzu::Data::ColumnIndex>
{
	[[nodiscard]] constexpr auto operator()(
	    const Vizzu::Data::ColumnIndex &index) const noexcept
	{
		return static_cast<uint64_t>(index);
	}
};

#endif
