#ifndef LIB_JSWRAPPERS_H
#define LIB_JSWRAPPERS_H

#include <memory>

namespace Vizzu
{

// Unlike std::unique_ptr, the deleter of std::shared_ptr is invoked
// even if the managed pointer is null -> keep unique_ptr
template <class R, class... Ts>
using JsFunctionWrapper =
    std::unique_ptr<R(std::remove_reference_t<Ts> *...),
        void (*)(R(std::remove_reference_t<Ts> *...))>;

template <class T> struct JsCompositionWrapper
{
	T values;
	void (*deleter)();
	~JsCompositionWrapper() { deleter(); }
	[[nodiscard]] JsCompositionWrapper(T &&values,
	    void (*deleter)()) :
	    values(std::move(values)),
	    deleter(deleter)
	{}
	JsCompositionWrapper(const JsCompositionWrapper &) = delete;
	JsCompositionWrapper(JsCompositionWrapper &&) noexcept = default;
	JsCompositionWrapper &operator=(
	    const JsCompositionWrapper &) = delete;
	JsCompositionWrapper &operator=(
	    JsCompositionWrapper &&) noexcept = default;
};

}

#endif
