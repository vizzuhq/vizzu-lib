#ifndef UTIL_EVENTDISPATCHER
#define UTIL_EVENTDISPATCHER

#include <forward_list>
#include <functional>
#include <map>
#include <memory>
#include <string>

#include "base/conv/auto_json.h"

#include "event.h"

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
	using event_ptr = std::shared_ptr<Event>;
	using stored_event_ptr = std::weak_ptr<Event>;

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

	class Event : public Util::Event<Params, const std::string>
	{
		friend class EventDispatcher;

	public:
		bool invoke(Params &&params = Params{}) const;

	private:
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
