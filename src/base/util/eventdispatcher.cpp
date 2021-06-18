#include "eventdispatcher.h"

using namespace Util;

EventDispatcher::handler_id EventDispatcher::Event::nextId = 1;

EventDispatcher::Sender::~Sender() {
}

std::string EventDispatcher::Sender::toJsonString() {
    return "{ \"instance\": 00000000 }";
}

EventDispatcher::Params::Params(const sender_ptr& sptr) {
    sender = sptr;
    stopPropagation = false;
}

std::string EventDispatcher::Params::toJsonString() {
    return "{ \"event\": { \"name\": \"test.event\" }, \"sender\": { \"instance\": 0 } }";
}

void EventDispatcher::Params::fromJsonString(const char*) {
}

EventDispatcher::Params::~Params() {
}

EventDispatcher::Event::Event(EventDispatcher& owner, const char* name)
    : owner(owner)
{
    active = true;
    this->uniqueName = name;
}

EventDispatcher::Event::~Event() {
}

const std::string EventDispatcher::Event::name() const {
    return uniqueName;
}

void EventDispatcher::Event::deactivate() {
    active = false;
}

void EventDispatcher::Event::invoke(Params& params) const {
    params.event = std::const_pointer_cast<Event>(shared_from_this());
    for(auto& handler : handlers) {
        //try
        params.handler = handler.first;
        handler.second(params);
        if (params.stopPropagation)
            break;
    }
    //delay remove from handler list if detach has happend during event processing
}

EventDispatcher::handler_id EventDispatcher::Event::attach(handler_fn handler) {
    handlers.push_back(std::make_pair(nextId, handler));
    return nextId++;
}

void EventDispatcher::Event::detach(handler_id id) {
    for(auto iter = handlers.begin(); iter != handlers.end(); iter++) {
        if (iter->first == id) {
            handlers.erase(iter);
            break;
        }
    }
}

EventDispatcher::Event::operator bool() const {
    return active && handlers.size();
}

void EventDispatcher::Event::operator()(Params& params) const {
    invoke(params);
}

EventDispatcher::~EventDispatcher() {
    for(auto& event : eventRegistry) {
        auto tmp = std::const_pointer_cast<Event>(event.second);
        tmp->deactivate();
    }
}

const EventDispatcher::event_ptr EventDispatcher::getEvent(const char* name) {
    auto iter = eventRegistry.find(name);
    if (iter == eventRegistry.end())
        return event_ptr{};
    return iter->second;
}

EventDispatcher::event_ptr EventDispatcher::operator[](const char* name) {
    return getEvent(name);
}

const EventDispatcher::event_ptr EventDispatcher::createEvent(const char* name) {
    auto iter = eventRegistry.find(name);
    if (iter != eventRegistry.end())
        return iter->second;
    event_ptr event;
    event = std::make_shared<Event>(*this, name);
    eventRegistry.insert(std::make_pair(name, event));
    return event;
}

bool EventDispatcher::destroyEvent(const char* name) {
    auto iter = eventRegistry.find(name);
    if (iter == eventRegistry.end())
        return false;
    auto tmp = std::const_pointer_cast<Event>(iter->second);
    tmp->deactivate();
    eventRegistry.erase(iter);
    return true;
}

bool EventDispatcher::destroyEvent(const event_ptr& event) {
    return destroyEvent(event->name().c_str());
}

void EventDispatcher::registerHandler(uint64_t owner, handler_id id) {
    auto iter = handlerRegistry.find(owner);
    if (iter == handlerRegistry.end()) {
        handlerRegistry.insert(std::make_pair(owner, std::list<handler_id>{}));
        iter = handlerRegistry.find(owner);
    }
    iter->second.push_back(id);
}

void EventDispatcher::unregisterHandler(const event_ptr& event, uint64_t owner) {
    auto tmp = std::const_pointer_cast<Event>(event);
    auto iter = handlerRegistry.find(owner);
    if (iter != handlerRegistry.end()) {
        for(auto& item : iter->second)
            tmp->detach((handler_id)item);
        handlerRegistry.erase(iter);
    }
}
