#ifndef UTIL_EVENT
#define UTIL_EVENT

#include <functional>
#include <list>
#include <memory>

namespace Util
{

template <typename... ParamTypes> class Event
{
public:
	typedef std::function<void(ParamTypes...)> Listener;
	struct Record
	{
		Record(Listener listener, bool once) :
		    listener(listener),
		    once(once)
		{}
		Listener listener;
		bool once;
	};
	typedef std::list<Record> Handlers;
	typedef typename Handlers::const_reverse_iterator Handle;

	void operator()(ParamTypes... params)
	{
		auto act = handlers.begin();
		while (act != handlers.end()) {
			auto next = act; // store next in case of act removes
			                 // itself from the list
			next++;
			if (act->listener) act->listener(params...);
			if (act->once) handlers.erase(act);
			act = next;
		}
	}

	Handle attach(const Listener &listener, bool once = false)
	{
		handlers.emplace_back(listener, once);
		return handlers.crbegin();
	}

	void detach(Handle handle) { handlers.erase(handle); }

	void detachAll() { handlers.clear(); }

private:
	Handlers handlers;
};

}

#endif
