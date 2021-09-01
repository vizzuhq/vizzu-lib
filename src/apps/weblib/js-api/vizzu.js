"use strict";

import Render from './render.js';
import Events from './events.js';
import Data from './data.js';
import AnimControl from './animcontrol.js';
import VizzuModule from './cvizzu.js';

export default class Vizzu
{
	constructor(container)
	{
		this.container = container;
		this.started = false;

		this.initializing = VizzuModule().then((module) => {
			return this.init(module);
		});

		this.snapshotRegistry = new FinalizationRegistry(snapshot => {
			this.call(this.module._chart_free)(snapshot);
		});
	}

	call(f)
	{
		return (...params) => {
			try { return f(...params); }
			catch(e) {
				let address = parseInt(e);
				let cMessage = this.module._vizzu_errorMessage(address);
				let message = this.module.UTF8ToString(cMessage);
				throw new Error('error: ' + message);
			}
		}
	}

	iterateObject(obj, paramHandler, path = '')
	{
		if (obj === null || obj === undefined) return;

		Object.keys(obj).forEach(key => {
			let newPath = path + (path.length === 0 ? "" : "." ) + key;
			if (obj[key] !== null && typeof obj[key] === 'object') {
				this.iterateObject(obj[key], paramHandler, newPath);
			} else {
				this.setValue(newPath, obj[key], paramHandler);
			}
		})
	}

	setNestedProp(obj, path, value) 
	{
		let propList = path.split('.');
		let propName;
		while ((propName = propList.shift()) !== undefined)
		{
			if (propList.length > 0) obj = obj[propName] ??= {};
			else obj[propName] = value.startsWith('[') 
				? JSON.parse(value) : value;
			// todo: detecting JSon here is only a workaround, 
			//       we should use a format parameter instead  
		}
	}

	setValue(path, value, setter)
	{
		if (typeof path !== 'string' && ! (path instanceof String))
			throw new Error('first parameter should be string');

		let cpath = this.toCString(path);
		let cvalue = this.toCString(String(value).toString());

		try {
			setter(cpath, cvalue);
		}
		finally
		{
			this.module._free(cvalue);
			this.module._free(cpath);
		}
	}

	setStyle(style)
	{
		if (style == null) style = { '': null };

		this.iterateObject(style, (path, value) => {
			this.call(this.module._style_setValue)(path, value);
		});
	}

	cloneObject(lister, getter)
	{
		let clistStr = this.call(lister)();
		let listStr = this.fromCString(clistStr); 
		let list = JSON.parse(listStr);
		let res = {}
		for (let path of list) 
		{
			let cpath = this.toCString(path);
			let cvalue;
			try {
				cvalue = this.call(getter)(cpath);
				let value = this.fromCString(cvalue);
				this.setNestedProp(res, path, value);
			}
			finally
			{
				this.module._free(cpath);
			}
		}
		Object.freeze(res);
		return res;
	}

	get config()
	{
		return this.cloneObject(
			this.module._chart_getList,
			this.module._chart_getValue);
	}

	get styles() 
	{
		return this.cloneObject(
			this.module._style_getList,
			this.module._style_getValue);
	}

	setConfig(config)
	{
		if (config !== null 
			&& typeof config === 'object'
			&& config.channels !== undefined
			&& config.channels !== null
			&& typeof config.channels === 'object')
		{
			let channels = config.channels;
			for (const ch in channels)
			{
				if (typeof channels[ch] === 'string') 
					channels[ch] = [ channels[ch] ];

				if (channels[ch] === null 
					|| Array.isArray(channels[ch])) 
					channels[ch] = { set: channels[ch] };

				if (typeof channels[ch].attach === 'string')
					channels[ch].attach = [ channels[ch].attach ];

				if (typeof channels[ch].detach === 'string')
					channels[ch].detach = [ channels[ch].detach ];

				if (typeof channels[ch].set === 'string')
					channels[ch].set = [ channels[ch].set ];

				if (Array.isArray(channels[ch].set)
				    && channels[ch].set.length == 0)
					channels[ch].set = null;
			}
		}

		this.iterateObject(config, (path, value) => {
			this.call(this.module._chart_setValue)(path, value);
		});
	}

	on(eventName, handler) {
		this.events.add(eventName, handler);
	}

	off(eventName, handler) {
		this.events.remove(eventName, handler);
	}

	store() {
		let id = this.call(this.module._chart_store)();
		let snapshot = { id };
		this.snapshotRegistry.register(snapshot, id);
		return snapshot;
	}

	restore(snapshot) 
	{
		this.call(this.module._chart_restore)(snapshot.id);
	}

	animate(obj, animOptions)
	{
		if (obj !== null && obj !== undefined && typeof obj === 'object')
		{
			if (obj.id !== undefined) this.restore(obj);
			else 
			{
				if (!obj.hasOwnProperty('data')
					&& !obj.hasOwnProperty('style')
					&& !obj.hasOwnProperty('config'))
				{
					obj = { config: obj };
				}

				this.data.set(obj.data);
				this.setStyle(obj.style);
				this.setConfig(obj.config);
			}
		}

		this.setAnimation(animOptions);

		return new Promise((resolve, reject) => {
			let callbackPtr = this.module.addFunction(() => {
				resolve(this);
				this.module.removeFunction(callbackPtr);
			}, 'v');
			this.call(this.module._chart_animate)(callbackPtr);
		});
	}

	setAnimation(animOptions)
	{
		if (animOptions !== undefined) 
		{
			if (animOptions === null) {
				animOptions = { duration: 0 };
			}
			else if (typeof animOptions === 'string'
				|| typeof animOptions === 'number')
			{
				animOptions = { duration: animOptions };
			}

			if (typeof animOptions === 'object') 
			{
				this.iterateObject(animOptions, (path, value) => {
					this.call(this.module._anim_setValue)(path, value);
				});
			}
			else throw new Error('invalid animation option');
		}
	}

	get animation() {
		return new AnimControl(this);
	}

	version()
	{
		let versionCStr = this.module._vizzu_version();
		let versionStr = this.module.UTF8ToString(versionCStr);
		return versionStr;
	}

	start() {
		if (!this.started)
		{
			this.call(this.module._vizzu_poll)();
			this.render.updateFrame(false);

			setInterval(()=>{
				this.call(this.module._vizzu_poll)();
			}, 10);

			setInterval(()=>{
				this.render.updateFrame(false);
			}, 25);

			this.started = true;
		}
	}

	getMousePos(evt)
	{
		var rect = this.render.clientRect();
		return([
			evt.clientX - rect.left,
			evt.clientY - rect.top
		]);
	}

	toCString(str)
	{
		let len = str.length * 4 + 1;
		let buffer = this.module._malloc(len);
		this.module.stringToUTF8(str, buffer, len);
		return buffer;
	}

	fromCString(str) {
		return this.module.UTF8ToString(str)
	}

	init(module)
	{
		this.module = module;

		let canvas = this.createCanvas();

		this.render = new Render;
		this.module.render = this.render;
		this.data = new Data(this);
		this.events = new Events(this);
		this.module.events = this.events;
		this.render.init(this.call(this.module._vizzu_update), canvas, false);
		this.call(this.module._vizzu_init)();
		this.call(this.module._vizzu_setLogging)(false);

		this.setupDOMEventHandlers(canvas);

		this.start();

		return this;
	}

	createCanvas() 
	{
		let canvas = null;
		let placeholder = this.container;

		if (!(placeholder instanceof HTMLElement)) {
			placeholder = document.getElementById(placeholder);
		}

		if (!placeholder) {
			throw new Error
				(`Cannot find container ${this.container} to render Vizzu!`);
		}

		if (placeholder instanceof HTMLCanvasElement) {
			canvas = placeholder;
		} else {
			canvas = document.createElement('CANVAS');
			canvas.style.width = "100%";
			canvas.style.height = "100%";
			placeholder.appendChild(canvas);
		}

		if (!(canvas instanceof HTMLCanvasElement)) {
			throw new Error("Error initializing <canvas> for Vizzu!");
		}

		return canvas;
	}

	setupDOMEventHandlers(canvas) 
	{
		this.resizeObserver = new ResizeObserver(entries => {
			this.render.updateFrame(true);
		});

		this.resizeObserver.observe(canvas);

		window.addEventListener("resize", () => {
			this.render.updateFrame(true);
		});

		canvas.addEventListener('mousemove', (evt) => {
			let pos = this.getMousePos(evt);
			this.call(this.module._vizzu_mouseMove)(pos[0], pos[1]);
		});

		canvas.addEventListener('mouseup', (evt) => {
			let pos = this.getMousePos(evt);
			this.call(this.module._vizzu_mouseUp)(pos[0], pos[1]);
		});

		canvas.addEventListener('mousedown', (evt) => {
			let pos = this.getMousePos(evt);
			this.call(this.module._vizzu_mouseDown)(pos[0], pos[1]);
		});

		canvas.addEventListener('mouseout', (evt) => {
			this.call(this.module._vizzu_mouseLeave)();
		});

		document.addEventListener('keydown', (evt) => {
			let key = evt.keyCode <= 255 ? evt.keyCode : 0;
			const keys = [ 33, 34, 36, 35, 37, 39, 38, 40, 27, 9, 13, 46 ];
			for (let i = 0; i < keys.length; i++)
				if (evt.key == keys[i]) key = 256+i;
			if (key != 0)
			{
				this.call(this.module._vizzu_keyPress)
					(key, evt.ctrlKey, evt.altKey, evt.shiftKey);
			}
		});
	}
}
