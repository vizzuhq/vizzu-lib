#include "eventdispatcher.h"

#include "base/conv/auto_json.h"

namespace Util
{

EventDispatcher::Params::Params(const EventTarget *s) : target(s) {}

std::string EventDispatcher::Params::toJSON() const
{
	std::string res;
	appendToJSON(
	    Conv::JSONObj{res}("type", event->name())("target", target)
	        .key("detail"));
	return res;
}

void EventDispatcher::Params::appendToJSON(Conv::JSON &obj) const
{
	obj.json += "{}";
}

EventDispatcher::Params::~Params() = default;

EventDispatcher::Event::Event(EventDispatcher &owner,
    const char *name) :
    uniqueName(name),
    owner(owner)
{}

EventDispatcher::Event::~Event() = default;

std::string EventDispatcher::Event::name() const
{
	return uniqueName;
}

void EventDispatcher::Event::deactivate() { active = false; }

bool EventDispatcher::Event::invoke(Params &&params)
{
	params.event = shared_from_this();
	for (auto &handler : handlers) {
		params.handler = static_cast<int>(handler.first);
		currentlyInvoked = params.handler;
		handler.second(params);
		currentlyInvoked = 0;
	}
	for (auto &item : handlersToRemove) detach(item.first);
	handlersToRemove.clear();
	return !params.preventDefault;
}

void EventDispatcher::Event::attach(std::uint64_t id,
    handler_fn handler)
{
	handlers.emplace_back(id, std::move(handler));
}

void EventDispatcher::Event::detach(std::uint64_t id)
{
	if (currentlyInvoked != 0)
		handlersToRemove.emplace_back(currentlyInvoked, handler_fn{});
	else {
		for (auto iter = handlers.begin(); iter != handlers.end();
		     iter++) {
			if (iter->first == id) {
				handlers.erase(iter);
				break;
			}
		}
	}
}

EventDispatcher::Event::operator bool() const
{
	return active && !handlers.empty();
}

bool EventDispatcher::Event::operator()(Params &&params)
{
	return invoke(std::move(params));
}

EventDispatcher::~EventDispatcher()
{
	for (auto &event : eventRegistry) { event.second->deactivate(); }
}

EventDispatcher::event_ptr EventDispatcher::getEvent(const char *name)
{
	auto iter = eventRegistry.find(name);
	if (iter == eventRegistry.end()) return event_ptr{};
	return iter->second;
}

EventDispatcher::event_ptr EventDispatcher::createEvent(
    const char *name)
{
	auto iter = eventRegistry.find(name);
	if (iter != eventRegistry.end()) return iter->second;
	event_ptr event;
	event = std::make_shared<Event>(*this, name);
	eventRegistry.insert(std::make_pair(name, event));
	return event;
}

bool EventDispatcher::destroyEvent(const char *name)
{
	auto iter = eventRegistry.find(name);
	if (iter == eventRegistry.end()) return false;
	iter->second->deactivate();
	eventRegistry.erase(iter);
	return true;
}

bool EventDispatcher::destroyEvent(const event_ptr &event)
{
	return destroyEvent(event->name().c_str());
}

void EventDispatcher::registerHandler(uint64_t owner, handler_id id)
{
	auto iter = handlerRegistry.find(owner);
	if (iter == handlerRegistry.end()) {
		handlerRegistry.insert(
		    std::make_pair(owner, std::list<handler_id>{}));
		iter = handlerRegistry.find(owner);
	}
	iter->second.push_back(id);
}

void EventDispatcher::unregisterHandler(const event_ptr &event,
    uint64_t owner)
{
	auto iter = handlerRegistry.find(owner);
	if (iter != handlerRegistry.end()) {
		for (auto &item : iter->second) event->detach(item);
		handlerRegistry.erase(iter);
	}
}

}