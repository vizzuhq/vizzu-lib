"use strict";

export default class Events
{
	constructor(module) {
        this.module = module;
        this.eventHandlers = new Map();
    }

	toCString(str) {
		let len = str.length * 4 + 1;
		let buffer = this.module._malloc(len);
		this.module.stringToUTF8(str, buffer, len);
		return buffer;
	}

	fromCString(str) {
		return this.module.UTF8ToString(str)
	}

	add(eventName, handler) {
		if (typeof eventName !== 'string' && !(eventName instanceof String))
			throw 'first parameter should be string';
		let cname = this.toCString(eventName);
		try {
			let id = this.module._addEventListener(cname);
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
			let cname = this.toCString(eventName);
			let id = this.eventHandlers[handler].id;
			this.module._removeEventListener(cname, id);
		}
		finally {
			this.eventHandlers.delete(handler);
			this.module._free(cname);
		}
	}

    invoke(handlerId, param) {
		try {
			if(this.eventHandlers.has(handlerId)) {
				let eventParam = JSON.parse(this.fromCString(param));
				this.eventHandlers.get(handlerId)(eventParam);
				let ret = JSON.stringify(eventParam);
				return this.toCString(ret);
			}
		}
		catch(e) {
			console.log("exception in event handler: " + e);
		}
		return 0;
    }
}