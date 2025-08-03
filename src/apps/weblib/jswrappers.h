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
	~JsCompositionWrapper()
	{
		if (auto &&d = std::exchange(deleter, {})) d();
	}
	[[nodiscard]] JsCompositionWrapper(T &&values,
	    void (*deleter)()) :
	    values(std::move(values)),
	    deleter(deleter)
	{}
	JsCompositionWrapper(const JsCompositionWrapper &) = delete;
	JsCompositionWrapper(JsCompositionWrapper &&other) noexcept :
	    values(std::move(other.values)),
	    deleter(std::exchange(other.deleter, {}))
	{}
	JsCompositionWrapper &operator=(
	    const JsCompositionWrapper &) = delete;
	JsCompositionWrapper &operator=(
	    JsCompositionWrapper &&other) noexcept
	{
		if (this->deleter != other.deleter) {
			if (deleter) deleter();
			values = std::move(other.values);
			deleter = std::exchange(other.deleter, {});
		}
		return *this;
	}
};

}

#endif
