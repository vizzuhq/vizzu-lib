#include "eventdispatcher.h"

#include "base/conv/auto_json.h"

namespace Util
{

EventDispatcher::Params::Params(const EventTarget *target) :
    target(target)
{}

std::string EventDispatcher::Params::toJSON() const
{
	std::string res;
	appendToJSON(
	    Conv::JSONObj{res}("type", eventName)("target", target)
	        .key("detail"));
	return res;
}

void EventDispatcher::Params::appendToJSON(Conv::JSON &obj) const
{
	obj.json += "{}";
}

EventDispatcher::Params::~Params() = default;

bool EventDispatcher::Event::invoke(Params &&params) const
{
	params.eventName = name;

	operator()(params, params.toJSON());

	return !params.preventDefault;
}

EventDispatcher::~EventDispatcher() = default;

EventDispatcher::event_ptr EventDispatcher::getEvent(
    std::string_view name) const
{
	if (auto iter = eventRegistry.find(name);
	    iter != eventRegistry.end())
		return iter->second.lock();
	return {};
}

EventDispatcher::event_ptr EventDispatcher::createEvent(
    std::string &&name)
{
	event_ptr res;

	if (auto iter_place = eventRegistry.lower_bound(name);
	    iter_place == eventRegistry.end() || iter_place->first != name
	        || ![&res, &iter_place]() -> auto &
	    {
		    return res = iter_place->second.lock();
	    }()) {
		res = std::make_shared<Event>();
		res->name =
		    eventRegistry
		        .insert_or_assign(iter_place, std::move(name), res)
		        ->first;
	}

	return res;
}

}