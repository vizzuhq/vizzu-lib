import Render from "./render.js";
import Events from "./events.js";
import Data from "./data.js";
import AnimControl from "./animcontrol.js";
import Tooltip from "./tooltip.js";
import Presets from "./presets.js";
import VizzuModule from "./cvizzu.js";
import { getCSSCustomPropsForElement, propsToObject } from "./utils.js";

let vizzuOptions = null;

export default class Vizzu {
  static get presets() {
    return new Presets();
  }

  static options(options) {
    vizzuOptions = options;
  }

  constructor(container, initState) {
    this.container = container;

    if (!(this.container instanceof HTMLElement)) {
      this.container = document.getElementById(container);
    }

    if (!this.container) {
      throw new Error(
        `Cannot find container ${this.container} to render Vizzu!`
      );
    }

    this._propPrefix = "vizzu";
    this.started = false;

    this._resolveAnimate = null;
    this.initializing = new AnimControl((resolve) => {
      this._resolveAnimate = resolve;
    }, this);
    this.anim = this.initializing;

    let moduleOptions = {};

    if (vizzuOptions?.wasmUrl) {
      moduleOptions["locateFile"] = function (path) {
        if (path.endsWith(".wasm")) {
          return vizzuOptions.wasmUrl;
        }
        return path;
      };
    }

    // load module
    VizzuModule(moduleOptions).then((module) => {
      if (this._resolveAnimate) {
        this._resolveAnimate(this.init(module));
      }
    });

    if (initState) {
      this.animate(initState, 0);
    }

    this.snapshotRegistry = new FinalizationRegistry((snapshot) => {
      this.call(this.module._chart_free)(snapshot);
    });
  }

  call(f) {
    return (...params) => {
      try {
        return f(...params);
      } catch (e) {
        let address = parseInt(e, 10);
        let cMessage = this.module._vizzu_errorMessage(address);
        let message = this.module.UTF8ToString(cMessage);
        throw new Error("error: " + message);
      }
    };
  }

  iterateObject(obj, paramHandler, path = "") {
    if (obj) {
      Object.keys(obj).forEach((key) => {
        let newPath = path + (path.length === 0 ? "" : ".") + key;
        if (obj[key] !== null && typeof obj[key] === "object") {
          this.iterateObject(obj[key], paramHandler, newPath);
        } else {
          this.setValue(newPath, obj[key], paramHandler);
        }
      });
    }
  }

  setNestedProp(obj, path, value) {
    let propList = path.split(".");
    propList.forEach((prop, i) => {
      if (i < propList.length - 1) {
        obj[prop] =
          obj[prop] || (typeof propList[i + 1] === "number" ? [] : {});
        obj = obj[prop];
      } else {
        // TODO json "detection" is a temporary workaround
        //      we should use a `format` parameter instead
        obj[prop] =
          value.startsWith("[") || value.startsWith("{")
            ? JSON.parse(value)
            : value;
      }
    });
  }

  setValue(path, value, setter) {
    if (path !== "" + path) {
      throw new Error("first parameter should be string");
    }

    let cpath = this.toCString(path);
    let cvalue = this.toCString(String(value).toString());

    try {
      setter(cpath, cvalue);
    } finally {
      this.module._free(cvalue);
      this.module._free(cpath);
    }
  }

  setStyle(style) {
    this.iterateObject(style, (path, value) => {
      this.call(this.module._style_setValue)(path, value);
    });
  }

  cloneObject(lister, getter, ...args) {
    let clistStr = this.call(lister)();
    let listStr = this.fromCString(clistStr);
    let list = JSON.parse(listStr);
    let res = {};
    for (let path of list) {
      let cpath = this.toCString(path);
      let cvalue;
      try {
        cvalue = this.call(getter)(cpath, ...args);
        let value = this.fromCString(cvalue);
        this.setNestedProp(res, path, value);
      } finally {
        this.module._free(cpath);
      }
    }
    Object.freeze(res);
    return res;
  }

  get config() {
    return this.cloneObject(
      this.module._chart_getList,
      this.module._chart_getValue
    );
  }

  get style() {
    return this.cloneObject(
      this.module._style_getList,
      this.module._style_getValue,
      false
    );
  }

  getComputedStyle() {
    return this.cloneObject(
      this.module._style_getList,
      this.module._style_getValue,
      true
    );
  }

  get data() {
    let cInfo = this.call(this.module._data_metaInfo)();
    let info = this.fromCString(cInfo);
    return { series: JSON.parse(info) };
  }

  setConfig(config) {
    if (config !== null && typeof config === "object") {
      Object.keys(config).forEach((key) => {
        if (
          ["color", "lightness", "size", "label", "x", "y", "noop"].includes(
            key
          )
        ) {
          config.channels = config.channels || {};
          config.channels[key] = config[key];
          delete config[key];
        }
      });
    }

    if (config?.channels) {
      let channels = config.channels;
      Object.keys(channels).forEach((ch) => {
        // TODO check
        if (typeof channels[ch] === "string") {
          channels[ch] = [channels[ch]];
        }

        if (channels[ch] === null || Array.isArray(channels[ch])) {
          channels[ch] = { set: channels[ch] };
        }

        if (typeof channels[ch].attach === "string") {
          channels[ch].attach = [channels[ch].attach];
        }

        if (typeof channels[ch].detach === "string") {
          channels[ch].detach = [channels[ch].detach];
        }

        if (typeof channels[ch].set === "string") {
          channels[ch].set = [channels[ch].set];
        }

        if (Array.isArray(channels[ch].set) && channels[ch].set.length === 0) {
          channels[ch].set = null;
        }
      });
    }

    this.iterateObject(config, (path, value) => {
      this.call(this.module._chart_setValue)(path, value);
    });
  }

  on(eventName, handler) {
    this._validateModule();
    this.events.add(eventName, handler);
  }

  off(eventName, handler) {
    this._validateModule();
    this.events.remove(eventName, handler);
  }

  store() {
    this._validateModule();
    let id = this.call(this.module._chart_store)();
    let snapshot = { id };
    this.snapshotRegistry.register(snapshot, id);
    return snapshot;
  }

  restore(snapshot) {
    this._validateModule();
    this.call(this.module._chart_restore)(snapshot.id);
  }

  feature(name, enabled) {
    this._validateModule();
    if (name === "tooltip") {
      this.tooltip.enable(enabled);
    } else if (name === "logging") {
      this.call(this.module._vizzu_setLogging)(enabled);
    } else if (name === "rendering") {
      this.render.enabled = enabled;
    }
  }

  _validateModule() {
    if (!this.module) {
      throw new Error("Vizzu not initialized. Use `initializing` promise.");
    }
  }

  animate(...args) {
    this.anim = this.anim.then(() => this._animate(args));
    return this.anim;
  }

  _animate(args) {
    return new AnimControl((resolve, reject) => {
      let callbackPtr = this.module.addFunction((ok) => {
        if (ok) {
          resolve(this);
        } else {
          reject("animation canceled");
          this.anim = Promise.resolve(this);
        }
        this.module.removeFunction(callbackPtr);
      }, "vi");
      this._processAnimParams(...args);
      this.call(this.module._chart_animate)(callbackPtr);
    }, this);
  }

  _processAnimParams(animTarget, animOptions) {
    let targets = Array.isArray(animTarget) ? animTarget : [animTarget];
    let opts = Array.isArray(animOptions) ? animOptions : [animOptions];
    for (let i = 0; i < targets.length; i++) {
      let target = targets[i];
      let opt = opts.length == 1 ? opts[0] : opts[i];
      this._setKeyframe(target, opt);
    }
  }

  _setKeyframe(animTarget, animOptions) {
    if (animTarget) {
      let obj = Object.assign({}, animTarget);
      if (obj.id) {
        this.restore(obj);
      } else {
        if (!obj.data && obj.style === undefined && !obj.config) {
          obj = { config: obj };
        }

        this._data.set(obj.data);

        // setting style, including CSS properties
        if (obj.style === null) {
          obj.style = { "": null };
        }
        const style = JSON.parse(JSON.stringify(obj.style || {}));
        const props = getCSSCustomPropsForElement(
          this.container,
          this._propPrefix
        );
        this.setStyle(propsToObject(props, style, this._propPrefix));
        this.setConfig(Object.assign({}, obj.config));
      }
    }

    this._setAnimation(animOptions);

    this.call(this.module._chart_setKeyframe)();
  }

  _setAnimation(animOptions) {
    if (typeof animOptions !== "undefined") {
      if (animOptions === null) {
        animOptions = { duration: 0 };
      } else if (
        typeof animOptions === "string" ||
        typeof animOptions === "number"
      ) {
        animOptions = { duration: animOptions };
      }

      if (typeof animOptions === "object") {
        animOptions = Object.assign({}, animOptions);
        this.iterateObject(animOptions, (path, value) => {
          this.call(this.module._anim_setValue)(path, value);
        });
      } else {
        throw new Error("invalid animation option");
      }
    }
  }

  // keeping this one for backward compatibility
  get animation() {
    return this.anim;
  }

  version() {
    this._validateModule();
    let versionCStr = this.module._vizzu_version();
    let versionStr = this.module.UTF8ToString(versionCStr);
    return versionStr;
  }

  start() {
    if (!this.started) {
      this.call(this.module._vizzu_poll)();
      this.render.updateFrame(false);

      this._pollInterval = setInterval(() => {
        this.call(this.module._vizzu_poll)();
      }, 10);

      this._updateInterval = setInterval(() => {
        this.render.updateFrame(false);
      }, 25);

      this.started = true;
    }
  }

  getMousePos(evt) {
    var rect = this.render.clientRect();
    return [evt.clientX - rect.left, evt.clientY - rect.top];
  }

  toCString(str) {
    let len = str.length * 4 + 1;
    let buffer = this.module._malloc(len);
    this.module.stringToUTF8(str, buffer, len);
    return buffer;
  }

  fromCString(str) {
    return this.module.UTF8ToString(str);
  }

  init(module) {
    this.module = module;

    this.canvas = this.createCanvas();

    this.render = new Render();
    this.module.render = this.render;
    this._data = new Data(this);
    this.events = new Events(this);
    this.module.events = this.events;
    this.tooltip = new Tooltip(this);
    this.render.init(this.call(this.module._vizzu_update), this.canvas, false);
    this.call(this.module._vizzu_init)();
    this.call(this.module._vizzu_setLogging)(false);

    this.setupDOMEventHandlers(this.canvas);

    this.start();

    return this;
  }

  createCanvas() {
    let canvas = null;
    let placeholder = this.container;

    if (placeholder instanceof HTMLCanvasElement) {
      canvas = placeholder;
    } else {
      canvas = document.createElement("CANVAS");
      canvas.style.width = "100%";
      canvas.style.height = "100%";
      placeholder.appendChild(canvas);
    }

    if (!(canvas instanceof HTMLCanvasElement)) {
      throw new Error("Error initializing <canvas> for Vizzu!");
    }

    return canvas;
  }

  setupDOMEventHandlers(canvas) {
    this.resizeObserver = new ResizeObserver(() => {
      this.render.updateFrame(true);
    });

    this.resizeObserver.observe(canvas);

    this._resizeHandler = () => {
      this.render.updateFrame(true);
    };

    this._mousemoveHandler = (evt) => {
      let pos = this.getMousePos(evt);
      this.call(this.module._vizzu_mouseMove)(pos[0], pos[1]);
    };

    this._mouseupHandler = (evt) => {
      let pos = this.getMousePos(evt);
      this.call(this.module._vizzu_mouseUp)(pos[0], pos[1]);
    };

    this._mousedownHandler = (evt) => {
      let pos = this.getMousePos(evt);
      this.call(this.module._vizzu_mouseDown)(pos[0], pos[1]);
    };

    this._mouseoutHandler = () => {
      this.call(this.module._vizzu_mouseLeave)();
    };

    this._wheelHandler = (evt) => {
      this.call(this.module._vizzu_mousewheel)(evt.deltaY);
    };

    this._keydownHandler = (evt) => {
      let key = evt.keyCode <= 255 ? evt.keyCode : 0;
      const keys = [33, 34, 36, 35, 37, 39, 38, 40, 27, 9, 13, 46];
      for (let i = 0; i < keys.length; i++) {
        // TODO .find?
        if (evt.key === keys[i]) {
          key = 256 + i;
        }
      }
      if (key !== 0) {
        this.call(this.module._vizzu_keyPress)(
          key,
          evt.ctrlKey,
          evt.altKey,
          evt.shiftKey
        );
      }
    };

    window.addEventListener("resize", this._resizeHandler);
    canvas.addEventListener("mousemove", this._mousemoveHandler);
    canvas.addEventListener("mouseup", this._mouseupHandler);
    canvas.addEventListener("mousedown", this._mousedownHandler);
    canvas.addEventListener("mouseout", this._mouseoutHandler);
    canvas.addEventListener("wheel", this._wheelHandler);
    document.addEventListener("keydown", this._keydownHandler);
  }

  detach() {
    this.resizeObserver.disconnect();
    clearInterval(this._pollInterval);
    clearInterval(this._updateInterval);
    window.removeEventListener("resize", this._resizeHandler);
    this.canvas.removeEventListener("mousemove", this._mousemoveHandler);
    this.canvas.removeEventListener("mouseup", this._mouseupHandler);
    this.canvas.removeEventListener("mousedown", this._mousedownHandler);
    this.canvas.removeEventListener("mouseout", this._mouseoutHandler);
    this.canvas.removeEventListener("wheel", this._wheelHandler);
    document.removeEventListener("keydown", this._keydownHandler);
  }
}
