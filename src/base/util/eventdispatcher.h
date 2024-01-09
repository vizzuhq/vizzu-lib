#ifndef UTIL_EVENTDISPATCHER
#define UTIL_EVENTDISPATCHER

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>

#include "base/conv/auto_json.h"

namespace Util
{

class EventTarget
{
public:
	virtual ~EventTarget() = default;
	[[nodiscard]] virtual std::string toJSON() const = 0;
};

class EventDispatcher
{
public:
	class Event;
	class Params;
	friend class Event;
	using handler_id = int;
	using event_ptr = std::shared_ptr<Event>;
	using handler_fn = std::function<void(Params &)>;
	using event_map = std::map<std::string, event_ptr>;
	using handler_list =
	    std::list<std::pair<std::uint64_t, handler_fn>>;

	class Params
	{
	public:
		explicit Params(const EventTarget *sptr = nullptr);
		virtual ~Params();
		event_ptr event;
		const EventTarget *target;
		handler_id handler{0};
		bool preventDefault{false};

		[[nodiscard]] std::string toJSON() const;
		virtual void appendToJSON(Conv::JSON &obj) const;
	};

	class Event : public std::enable_shared_from_this<Event>
	{
		friend class EventDispatcher;

	public:
		Event(EventDispatcher &owner, const char *name);
		virtual ~Event();

		[[nodiscard]] const std::string &name() const;
		bool invoke(Params &&params = Params{});
		void attach(std::uint64_t id, handler_fn handler);
		void detach(std::uint64_t id);
		explicit operator bool() const;
		bool operator()(Params &&params);

		template <typename T> void attach(T &handlerOwner)
		{
			static_assert(!std::is_const_v<T>);
			attach(std::hash<T *>{}(std::addressof(handlerOwner)),
			    std::ref(handlerOwner));
		}

		template <typename T> void detach(T &handlerOwner)
		{
			static_assert(!std::is_const_v<T>);
			detach(std::hash<T *>{}(std::addressof(handlerOwner)));
		}

	protected:
		bool active{true};
		std::string uniqueName;
		handler_list handlers;
		EventDispatcher &owner;
		handler_id currentlyInvoked{0};
		handler_list handlersToRemove;

		void deactivate();
	};

	virtual ~EventDispatcher();

	event_ptr getEvent(const char *name);
	event_ptr createEvent(const char *name);
	bool destroyEvent(const char *name);
	bool destroyEvent(const event_ptr &event);

protected:
	event_map eventRegistry;
};

}

#endif
