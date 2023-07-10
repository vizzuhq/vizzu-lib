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
	explicit EventTarget(EventTarget *parent = nullptr) : parent(parent) {}
	virtual ~EventTarget() {}
	virtual std::string toJson() const;
private:
	EventTarget *parent;
};

class EventDispatcher
{
public:
	class Event;
	class Params;
	friend class Event;
	typedef int handler_id;
	typedef std::shared_ptr<Event> event_ptr;
	typedef std::function<void(Params &)> handler_fn;
	typedef std::map<std::string, event_ptr> event_map;
	typedef std::list<std::pair<std::uint64_t, handler_fn>> handler_list;
	typedef std::map<uint64_t, std::list<handler_id>> handler_map;

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
		void attach(std::uint64_t id, handler_fn handler);
		void detach(std::uint64_t id);
		operator bool() const;
		bool operator()(Params &&params);

	protected:
		bool active;
		std::string uniqueName;
		handler_list handlers;
		EventDispatcher &owner;
		handler_id currentlyInvoked;
		handler_list handlersToRemove;

		void deactivate();
	};

public:
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
