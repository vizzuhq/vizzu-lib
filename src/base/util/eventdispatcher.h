#ifndef UTIL_EVENTDISPATCHER
#define UTIL_EVENTDISPATCHER

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>

namespace Util
{

class EventTarget
{
public:
	explicit EventTarget() {}
	virtual ~EventTarget() = default;
	[[nodiscard]] virtual std::string toJson() const = 0;
protected:
};

class EventDispatcher
{
public:
	class Event;
	class Params;
	friend class Event;
	using handler_id = int;
	using event_ptr = std::shared_ptr<Event>;
	using handler_fn = std::function<void (Params &)>;
	using event_map = std::map<std::string, event_ptr>;
	using handler_list = std::list<std::pair<std::uint64_t, handler_fn>>;
	using handler_map = std::map<uint64_t, std::list<handler_id>>;

	class Params
	{
	public:
		Params(const EventTarget *sptr = nullptr);
		virtual ~Params();
		event_ptr event;
		const EventTarget *target;
		handler_id handler;
		bool stopPropagation;
		bool preventDefault;

		[[nodiscard]] std::string toJsonString() const;
		[[nodiscard]] virtual std::string dataToJson() const;
		virtual void jsonToData(const char *jstr);
	};

	class Event : public std::enable_shared_from_this<Event>
	{
		friend class EventDispatcher;

	public:
		Event(EventDispatcher &owner, const char *name);
		virtual ~Event();

		std::string name() const;
		bool invoke(Params &&params = Params());
		void attach(std::uint64_t id, handler_fn handler);
		void detach(std::uint64_t id);
		operator bool() const;
		bool operator()(Params &&params);

		template <typename T>
		void attach(T& handlerOwner)
		{
			static_assert(!std::is_const_v<T>);
			attach(std::hash<T*>{}(std::addressof(handlerOwner)),
			    std::ref(handlerOwner));
		}

		template <typename T> void detach(T &handlerOwner)
		{
			static_assert(!std::is_const_v<T>);
			detach(std::hash<T*>{}(std::addressof(handlerOwner)));
		}

	protected:
		bool active;
		std::string uniqueName;
		handler_list handlers;
		EventDispatcher &owner;
		handler_id currentlyInvoked;
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
	handler_map handlerRegistry;

	void registerHandler(uint64_t owner, handler_id id);
	void unregisterHandler(const event_ptr &event, uint64_t owner);
};

}

#endif
