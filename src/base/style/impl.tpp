#ifndef BASE_STYLE_IMPL_TPP_INCLUDED
#define BASE_STYLE_IMPL_TPP_INCLUDED

#include "parammerger.h"
#include "paramregistry.h"
#include "sheet.h"

namespace Style
{

template <class T> T Sheet<T>::getFullParams() const
{
	return ParamMerger::merge(T{defaultParams}, activeParams.get());
}

template <class T> ParamRegistry<T>::ParamRegistry()
{
	Refl::visit<T>(
	    [this]<IsAccessor<T> U>(U &&accessor,
	        const std::initializer_list<std::string_view> &thePath)
	    {
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
