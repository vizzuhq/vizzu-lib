#ifndef BASE_STYLE_IMPL_TPP_INCLUDED
#define BASE_STYLE_IMPL_TPP_INCLUDED

#include "base/text/smartstring.h"

#include "param.h"
#include "parammerger.h"
#include "sheet.h"

namespace Style
{
template <class U, class Root>
concept IsAccessor =
    IsParam<std::remove_cvref_t<std::invoke_result_t<U &&, Root &>>>;

template <class T> T Sheet<T>::getFullParams() const
{
	return ParamMerger::merge(T{defaultParams}, activeParams.get());
}

template <class Root> constexpr const auto &getAccessors()
{
	static const std::map accessors{[]
	    {
		    std::map<std::string,
		        Refl::Access::Accessor<Root, true>,
		        std::less<>>
		        accessors;
		    Refl::visit<Root>(
		        [&accessors]<IsAccessor<Root> U>(U &&accessor,
		            const std::initializer_list<std::string_view>
		                &thePath)
		        {
			        accessors.try_emplace(
			            Text::SmartString::join<'.'>(thePath),
			            Refl::Access::Accessor<Root, true>::make(
			                std::forward<U>(accessor)));
		        });
		    return accessors;
	    }()};
	return accessors;
}

}

#endif
