#ifndef BASE_UTIL_EVENT_H
#define BASE_UTIL_EVENT_H

#include <forward_list>
#include <functional>

namespace Util
{

template <typename... ParamTypes> class Event
{
protected:
	using handler_id = std::size_t;
	using handler_fn = std::function<void(ParamTypes &...)>;
	using handler_list =
	    std::forward_list<std::pair<handler_id, handler_fn>>;

public:
	void operator()(ParamTypes &...params) const
	{
		for (auto iter = handlers.begin(); iter != handlers.end();)
			iter++->second(params...);
	}

	template <class T> void attach(T &&listener, handler_id id = {})
	{
		handlers.push_front({id, std::forward<T>(listener)});
	}

	void detach(handler_id id)
	{
		for (auto oit = handlers.before_begin(), it = std::next(oit);
		     it != handlers.end();
		     oit = it++)
			if (it->first == id) {
				handlers.erase_after(oit);
				break;
			}
	}

	void detachAll() { handlers.clear(); }

protected:
	handler_list handlers;
};

}

#endif
