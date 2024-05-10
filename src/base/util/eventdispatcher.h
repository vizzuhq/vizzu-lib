#ifndef UTIL_EVENTDISPATCHER
#define UTIL_EVENTDISPATCHER

#include <forward_list>
#include <functional>
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
	using handler_hash = std::size_t;
	using event_ptr = std::shared_ptr<Event>;
	using stored_event_ptr = std::weak_ptr<Event>;
	using handler_fn = std::function<void(Params &)>;
	using handler_list =
	    std::forward_list<std::pair<handler_hash, handler_fn>>;

	class Params
	{
	public:
		explicit Params(const EventTarget *target = nullptr);
		virtual ~Params();
		std::string_view eventName;
		const EventTarget *target;
		bool preventDefault{false};

		[[nodiscard]] std::string toJSON() const;
		virtual void appendToJSON(Conv::JSON &obj) const;
	};

	class Event
	{
		friend class EventDispatcher;

	public:
		bool invoke(Params &&params = Params{});
		void attach(handler_hash id, handler_fn handler);
		void detach(handler_hash id);
		[[nodiscard]] bool operator()(Params &&params);

	private:
		handler_list handlers;
		std::string_view name;
	};

	virtual ~EventDispatcher();

	[[nodiscard]] event_ptr getEvent(std::string_view name) const;

	[[nodiscard]] event_ptr createEvent(std::string &&name);

protected:
	std::map<std::string, stored_event_ptr, std::less<>>
	    eventRegistry;
};

}

#endif
