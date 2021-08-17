"use strict";

export default class Events
{
	constructor(vizzu) {
		this.vizzu = vizzu;
        this.module = vizzu.module;
        this.eventHandlers = new Map();
    }

	add(eventName, handler) {
		if (typeof eventName !== 'string' && !(eventName instanceof String))
			throw new Error('first parameter should be string');
		let cname = this.vizzu.toCString(eventName);
		try {
			let id = this.vizzu.call(this.module._addEventListener)(cname);
			this.eventHandlers.set(id, handler);
		}
		finally {
			this.module._free(cname);
		}
	}

	remove(eventName, handler) {
		let cname = this.vizzu.toCString(eventName);
		if (typeof eventName !== 'string' && !(eventName instanceof String))
			throw new Error('first parameter should be string');
		try {
			if (typeof handler === 'undefined') {
				this.vizzu.call(this.module._removeEventListener)(cname, 0);
			}
			else {
				if (!this.eventHandlers.has(handler))
					throw new Error('unknown event handler');
				this.eventHandlers.forEach((value, key) => {
					if (value == handler)
						this.vizzu.call(this.module._removeEventListener)(cname, key);
				});
			}
		}
		finally {
			this.eventHandlers.delete(handler);
			this.module._free(cname);
		}
	}

	invoke(handlerId, param) {
		try {
			if(this.eventHandlers.has(handlerId)) {
				let eventParam = JSON.parse(this.vizzu.fromCString(param));
				eventParam.preventDefault = () => {
					this.vizzu.call(this.module._event_preventDefault)();
				};
				if (eventParam.event.endsWith('-draw'))
					eventParam.renderingContext = this.vizzu.render.dc();
				this.eventHandlers.get(handlerId)(eventParam);
			}
		}
		catch(e) {
			console.log("exception in event handler: " + e);
		}
	}
}