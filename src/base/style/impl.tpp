#ifndef BASE_STYLE_IMPL_TPP_INCLUDED
#define BASE_STYLE_IMPL_TPP_INCLUDED

#include "parammerger.h"
#include "paramregistry.h"
#include "sheet.h"

namespace Style
{

template <class T> T Sheet<T>::getFullParams() const
{
	if (!activeParams)
		throw std::logic_error("no active parameters set");

	return ParamMerger::merge(T{defaultParams}, *activeParams);
}

template <class T> ParamRegistry<T>::ParamRegistry()
{
	Refl::visit<T>([this]<class U,
	    std::enable_if_t<std::is_constructible_v<Accessor, U>> * =
	        nullptr>(U && accessor,
	    const std::initializer_list<std::string_view>& thePath = {}) {
		std::string currentPath;
		for (auto sv : thePath) {
			if (!currentPath.empty()) currentPath += '.';
			currentPath += sv;
		}

		accessors.try_emplace(std::move(currentPath),
		    std::forward<U>(accessor));
	});
}
}

#endif
