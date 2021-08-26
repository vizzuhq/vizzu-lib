"use strict";

import Render from './render.js';
import Events from './events.js';
import Data from './data.js';
import AnimControl from './animcontrol.js';
import VizzuModule from './cvizzu.js';

export default class Vizzu
{
	constructor(container, onLoaded)
	{
		this.container = container;
		this.onLoaded = onLoaded;
		this.started = false;

		this.initializing = VizzuModule().then((module) => {
			return this.init(module);
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
				throw "error: " + message;
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

	setValue(path, value, setter)
	{
		if (typeof path !== 'string' && ! (path instanceof String))
			throw 'first parameter should be string';

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
		this.iterateObject(style, (path, value) => {
			this.call(this.module._style_setValue)(path, value);
		});
	}

	setDescriptor(descriptor)
	{
		this.iterateObject(descriptor, (path, value) => {
			this.call(this.module._chart_setValue)(path, value);
		});
	}

	on(eventName, handler) {
		this.events.add(eventName, handler);
	}

	off(eventName, handler) {
		this.events.remove(eventName, handler);
	}

	animate(obj, animOptions)
	{
		if (obj !== null && obj !== undefined && typeof obj === 'object')
		{
			this.data.set(obj.data);
			this.setStyle(obj.style);
			this.setDescriptor(obj.descriptor);
		}

		if (animOptions !== null && animOptions !== undefined 
			&& typeof animOptions === 'object')
		{
			this.iterateObject(animOptions, (path, value) => {
				this.call(this.module._anim_setValue)(path, value);
			});
		}

		return new Promise((resolve, reject) => {
			let callbackPtr = this.module.addFunction(() => {
				resolve(this);
				this.module.removeFunction(callbackPtr);
			}, 'v');
			this.call(this.module._chart_animate)(callbackPtr);
		});
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
		let canvas = null;
		let placeholder = this.container;

		if (!(placeholder instanceof HTMLElement)) {
			placeholder = document.getElementById(placeholder);
		}

		if (!placeholder) {
			throw(`Cannot find container ${this.container} to render Vizzu!`);
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
			throw("Error initializing <canvas> for Vizzu!");
		}

		this.render = new Render;
		this.module.render = this.render;
		this.data = new Data(this);
		this.events = new Events(this);
		this.module.events = this.events;
		this.render.init(this.call(this.module._vizzu_update), canvas, false);
		this.call(this.module._vizzu_init)(96,400,300);
		this.call(this.module._vizzu_setLogging)(true);

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

		canvas.addEventListener('onmouseover', (evt) => {
			this.call(this.module._vizzu_mouseOver)();
			console.log("asdfasdf");
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

		if (this.onLoaded) {
			this.onLoaded();
		}
		this.start();

		return this;
	}
}

// vim: sts=0 noexpandtab
