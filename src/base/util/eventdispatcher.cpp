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

EventDispatcher::Event::Event(std::string_view name) :
    uniqueName(name)
{}

EventDispatcher::Event::~Event() = default;

const std::string_view &EventDispatcher::Event::name() const noexcept
{
	return uniqueName;
}

bool EventDispatcher::Event::invoke(Params &&params)
{
	params.eventName = name();

	for (auto iter = handlers.begin(); iter != handlers.end();)
		iter++->second(params);

	return !params.preventDefault;
}

void EventDispatcher::Event::attach(handler_hash id,
    handler_fn handler)
{
	handlers.emplace_front(id, std::move(handler));
}

void EventDispatcher::Event::detach(handler_hash id)
{
	for (auto oit = handlers.before_begin(), it = std::next(oit);
	     it != handlers.end();
	     oit = it++)
		if (it->first == id) {
			handlers.erase_after(oit);
			break;
		}
}

bool EventDispatcher::Event::operator()(Params &&params)
{
	return invoke(std::move(params));
}

EventDispatcher::~EventDispatcher() = default;

EventDispatcher::event_ptr EventDispatcher::getEvent(
    std::string_view name) const
{
	if (auto iter = eventRegistry.find(name);
	    iter != eventRegistry.end())
		return *iter;
	return {};
}

const EventDispatcher::event_ptr &EventDispatcher::createEvent(
    std::string_view name)
{
	auto iter_place = eventRegistry.lower_bound(name);
	if (iter_place == eventRegistry.end()
	    || (*iter_place)->name() != name)
		iter_place = eventRegistry.insert(iter_place,
		    std::make_shared<Event>(name));

	return *iter_place;
}

bool EventDispatcher::destroyEvent(const event_ptr &event)
{
	auto iter = eventRegistry.find(event->name());
	if (iter == eventRegistry.end()) return false;
	eventRegistry.erase(iter);
	return true;
}
}