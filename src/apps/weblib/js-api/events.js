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
			throw 'first parameter should be string';
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
		if (typeof eventName !== 'string' && !(eventName instanceof String))
			throw 'first parameter should be string';
		if (!this.eventHandlers.has(handler))
			throw "unknown event handler";
		try {
			let cname = this.vizzu.toCString(eventName);
			this.eventHandlers.forEach((value, key) => {
				if (value == handler)
					this.vizzu.call(this.module._removeEventListener)(cname, key);
			});
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
				this.eventHandlers.get(handlerId)(eventParam);
				let ret = JSON.stringify(eventParam);
				return this.vizzu.toCString(ret);
			}
		}
		catch(e) {
			console.log("exception in event handler: " + e);
		}
		return 0;
    }
}