#ifndef UTIL_EVENTDISPATCHER
#define UTIL_EVENTDISPATCHER

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>

namespace Util
{

class EventDispatcher
{
public:
	class Event;
	class Params;
	class Sender;
	friend class Event;
	typedef int handler_id;
	typedef std::shared_ptr<Event> event_ptr;
	typedef std::function<void(Params &)> handler_fn;
	typedef std::map<std::string, event_ptr> event_map;
	typedef std::list<std::pair<handler_id, handler_fn>> handler_list;
	typedef std::map<uint64_t, std::list<handler_id>> handler_map;

	class Sender
	{
	public:
		virtual ~Sender();
		virtual std::string toJsonString() const;
	};

	class Params
	{
	public:
		Params(const Sender *sptr = nullptr);
		virtual ~Params();
		event_ptr event;
		const Sender *sender;
		handler_id handler;
		bool stopPropagation;
		bool preventDefault;

		std::string toJsonString() const;
		virtual std::string dataToJson() const;
		virtual void jsonToData(const char *jstr);
	};

	class Event : public std::enable_shared_from_this<Event>
	{
		friend class EventDispatcher;

	public:
		Event(EventDispatcher &owner, const char *name);
		virtual ~Event();

		const std::string name() const;
		bool invoke(Params &&params = Params());
		handler_id attach(handler_fn handler);
		void detach(handler_id id);
		operator bool() const;
		bool operator()(Params &&params);

		template <typename T>
		handler_id attach(const T &handlerOwner, handler_fn handler)
		{
			auto id = attach(handler);
			owner.registerHandler(
			    std::hash<const T*>{}(std::addressof(handlerOwner)), id);
			return id;
		}

		template <typename T> void detach(const T &handlerOwner)
		{
			owner.unregisterHandler(shared_from_this(),
			    std::hash<const T*>{}(std::addressof(handlerOwner)));
		}

	protected:
		bool active;
		std::string uniqueName;
		handler_list handlers;
		static handler_id nextId;
		EventDispatcher &owner;
		handler_id currentlyInvoked;
		handler_list handlersToRemove;

		void deactivate();
	};

public:
	virtual ~EventDispatcher();

	const event_ptr getEvent(const char *name);
	event_ptr operator[](const char *name);
	const event_ptr createEvent(const char *name);
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
