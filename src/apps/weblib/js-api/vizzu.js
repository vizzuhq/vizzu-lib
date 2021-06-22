"use strict";

import Render from './render.js';
import Events from './events.js';
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

	setData(obj)
	{
		if (obj === null || obj === undefined) return;
		if (obj.series === undefined
			|| obj.series === null
			|| !Array.isArray(obj.series))

			throw new Error('data series field missing or not an array');

		for (const series of obj.series) this.setSeries(series);
	}

	setSeries(series)
	{
		if (series.name === 'undefined')
			throw new Error('missing series name');

		if (series.values === 'undefined')
			throw new Error('missing series values');

		if(series.type === 'categories')
			this.addCategories(series.name, series.values);

		else if (series.type === 'values')
			this.addValues(series.name, series.values);

		else throw new Error('invalid series type: ' + series.type);
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

	addCategories(name, categories)
	{
		if (typeof name !== 'string' && ! (name instanceof String))
			throw 'first parameter should be string';

		if ( !(categories instanceof Array))
			throw 'second parameter should be an array';

		let ptrs = new Uint32Array(categories.length);
		for (let i = 0; i < categories.length; i++)
		{
			if (typeof categories[i] !== 'string'
				&& ! (categories[i] instanceof String))
				throw 'array element should be string';

			let ptr = this.toCString(categories[i]);
			ptrs[i] = ptr;
		}

		let ptrArrayLen = categories.length * 4;

		let ptrArr = this.module._malloc(ptrArrayLen);
		var ptrHeap = new Uint8Array(this.module.HEAPU8.buffer, ptrArr, ptrArrayLen);
		ptrHeap.set(new Uint8Array(ptrs.buffer));

		let cname = this.toCString(name);

		try
		{
			this.call(this.module._data_addCategories)(cname, ptrArr, categories.length);
		}
		finally
		{
			this.module._free(cname);
			for (let ptr of ptrs) this.module._free(ptr);
			this.module._free(ptrArr);
		}
	}

	addValues(name, values)
	{
		if (typeof name !== 'string' && ! (name instanceof String))
			throw 'first parameter should be string';

		if ( !(values instanceof Array))
			throw 'second parameter should be an array';

		let vals = new Float64Array(values);
		let valArrayLen = values.length * 8;

		let valArr = this.module._malloc(valArrayLen);
		var valHeap = new Uint8Array(this.module.HEAPU8.buffer, valArr, valArrayLen);
		valHeap.set(new Uint8Array(vals.buffer));

		let cname = this.toCString(name);

		try
		{
			this.call(this.module._data_addValues)(cname, valArr, values.length);
		}
		finally
		{
			this.module._free(cname);
			this.module._free(valArr);
		}
	}

	addEventListener(eventName, handler) {
		this.events.add(eventName, handler);
	}

	removeEventListener(eventName, handler) {
		this.events.remove(eventName, handler);
	}

	animate(obj)
	{
		if (obj !== null && obj !== undefined && typeof obj === 'object')
		{
			this.setData(obj.data);
			this.setStyle(obj.style);
			this.setDescriptor(obj.descriptor);
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
		this.events = new Events(this);
		this.module.events = this.events;
		this.render.init(this.call(this.module._vizzu_update), canvas, false);
		this.call(this.module._vizzu_init)(96,400,300);
		this.call(this.module._vizzu_setLogging)(true);

		this.resizeObserver = new ResizeObserver(entries => {
			this.render.updateFrame(true);
		});

		this.resizeObserver.observe(canvas);

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
