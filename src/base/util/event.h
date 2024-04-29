#ifndef BASE_UTIL_EVENT_H
#define BASE_UTIL_EVENT_H

#include <functional>
#include <memory>

namespace Util
{

template <typename... ParamTypes> class Event
{
public:
	using Listener = std::function<void(const ParamTypes &...)>;

	struct CompositeListener
	{
		inline void operator()(const ParamTypes &...params) const
		{
			listener1(params...);
			listener2(params...);
		}

		Listener listener1;
		Listener listener2;
	};

	void operator()(const ParamTypes &...params) const
	{
		listens(params...);
	}

	void attach(const Listener &listener)
	{
		if (listens) {
			listens = CompositeListener{std::move(listens), listener};
		}
		else {
			listens = listener;
		}
	}

	void detachAll() { listens = nullptr; }

private:
	Listener listens;
};

}

#endif
