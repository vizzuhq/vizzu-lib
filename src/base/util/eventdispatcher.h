#ifndef UTIL_EVENTDISPATCHER
#define UTIL_EVENTDISPATCHER

#include <forward_list>
#include <functional>
#include <memory>
#include <set>
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

	class Event : public std::enable_shared_from_this<Event>
	{
		friend class EventDispatcher;

	public:
		explicit Event(std::string_view name);
		virtual ~Event();

		[[nodiscard]] const std::string_view &name() const noexcept;
		bool invoke(Params &&params = Params{});
		void attach(handler_hash id, handler_fn handler);
		void detach(handler_hash id);
		[[nodiscard]] bool operator()(Params &&params);

	protected:
		std::string_view uniqueName;
		handler_list handlers;
	};

	virtual ~EventDispatcher();

	[[nodiscard]] event_ptr getEvent(std::string_view name) const;

	[[nodiscard]] const event_ptr &createEvent(std::string_view name);

	bool destroyEvent(const event_ptr &event);

protected:
	struct EventPtrComp
	{
		using is_transparent = std::true_type;
		[[nodiscard]] bool operator()(const event_ptr &lhs,
		    const std::string_view &rhs) const noexcept
		{
			return lhs->name() < rhs;
		}

		[[nodiscard]] bool operator()(const std::string_view &lhs,
		    const event_ptr &rhs) const noexcept
		{
			return lhs < rhs->name();
		}

		[[nodiscard]] bool operator()(const event_ptr &lhs,
		    const event_ptr &rhs) const noexcept
		{
			return lhs->name() < rhs->name();
		}
	};

	std::set<event_ptr, EventPtrComp> eventRegistry;
};

}

#endif
