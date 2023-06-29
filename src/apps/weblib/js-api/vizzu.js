import Render from "./render.js";
import Events from "./events.js";
import Data from "./data.js";
import { Animation, AnimControl } from "./animcontrol.js";
import Tooltip from "./tooltip.js";
import Presets from "./presets.js";
import VizzuModule from "./cvizzu.js";
import { getCSSCustomPropsForElement, propsToObject } from "./utils.js";
import ObjectRegistry from "./objregistry.js";

let vizzuOptions = null;

class Snapshot {}

export default class Vizzu {
  static get presets() {
    return new Presets();
  }

  static options(options) {
    vizzuOptions = options;
  }

  constructor(container, initState) {
    this._container = container;

    if (!(this._container instanceof HTMLElement)) {
      this._container = document.getElementById(container);
    }

    if (!this._container) {
      throw new Error(
        `Cannot find container ${this._container} to render Vizzu!`
      );
    }

    this._propPrefix = "vizzu";
    this._started = false;

    this._resolveAnimate = null;
    this.initializing = new Promise((resolve) => {
      this._resolveAnimate = resolve;
    });
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
        this._resolveAnimate(this._init(module));
      }
    });

    if (initState) {
      this.animate(initState, 0);
    }
  }

  _call(f) {
    return (...params) => {
      try {
        return f(...params);
      } catch (e) {
        if (Number.isInteger(e)) {
          let address = parseInt(e, 10);
          let type = new this.module['ExceptionInfo'](address)
          let cMessage = this.module._vizzu_errorMessage(address, type.get_type());
          let message = this.module.UTF8ToString(cMessage);
          throw new Error("error: " + message);
        } else {
          throw e;
        }
      }
    };
  }

  _iterateObject(obj, paramHandler, path = "") {
    if (obj) {
      Object.keys(obj).forEach((key) => {
        let newPath = path + (path.length === 0 ? "" : ".") + key;
        if (obj[key] !== null && typeof obj[key] === "object") {
          this._iterateObject(obj[key], paramHandler, newPath);
        } else {
          this._setValue(newPath, obj[key], paramHandler);
        }
      });
    }
  }

  /* Note: If the value string containing a JSON, it will be parsed. */
  _setNestedProp(obj, path, value) {
    let propList = path.split(".");
    propList.forEach((prop, i) => {
      if (i < propList.length - 1) {
        obj[prop] =
          obj[prop] || (typeof propList[i + 1] === "number" ? [] : {});
        obj = obj[prop];
      } else {
        obj[prop] =
          value.startsWith("[") || value.startsWith("{")
            ? JSON.parse(value)
            : value;
      }
    });
  }

  _setValue(path, value, setter) {
    if (path !== "" + path) {
      throw new Error("first parameter should be string");
    }

    let cpath = this._toCString(path);
    let cvalue = this._toCString(String(value).toString());

    try {
      setter(cpath, cvalue);
    } finally {
      this.module._free(cvalue);
      this.module._free(cpath);
    }
  }

  _setStyle(style) {
    this._iterateObject(style, (path, value) => {
      this._call(this.module._style_setValue)(path, value);
    });
  }

  _cloneObject(lister, getter, ...args) {
    let clistStr = this._call(lister)();
    let listStr = this._fromCString(clistStr);
    let list = JSON.parse(listStr);
    let res = {};
    for (let path of list) {
      let cpath = this._toCString(path);
      let cvalue;
      try {
        cvalue = this._call(getter)(cpath, ...args);
        let value = this._fromCString(cvalue);
        this._setNestedProp(res, path, value);
      } finally {
        this.module._free(cpath);
      }
    }
    Object.freeze(res);
    return res;
  }

  get config() {
    return this._cloneObject(
      this.module._chart_getList,
      this.module._chart_getValue
    );
  }

  get style() {
    return this._cloneObject(
      this.module._style_getList,
      this.module._style_getValue,
      false
    );
  }

  getComputedStyle() {
    return this._cloneObject(
      this.module._style_getList,
      this.module._style_getValue,
      true
    );
  }

  get data() {
    let cInfo = this._call(this.module._data_metaInfo)();
    let info = this._fromCString(cInfo);
    return { series: JSON.parse(info) };
  }

  _setConfig(config) {
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

    this._iterateObject(config, (path, value) => {
      this._call(this.module._chart_setValue)(path, value);
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
    return this._objectRegistry.get(
      this._call(this.module._chart_store),
      Snapshot
    );
  }

  feature(name, enabled) {
    this._validateModule();
    if (name === "tooltip") {
      this._tooltip.enable(enabled);
    } else if (name === "logging") {
      this._call(this.module._vizzu_setLogging)(enabled);
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
    let activate;
    let activated = new Promise((resolve, reject) => {
      activate = resolve;
    });
    this.anim = this.anim.then(() => this._animate(args, activate));
    this.anim.activated = activated;
    return this.anim;
  }

  _animate(args, activate) {
    let anim = new Promise((resolve, reject) => {
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
      this._call(this.module._chart_animate)(callbackPtr);
    }, this);
    activate(new AnimControl(this));
    return anim;
  }

  _processAnimParams(animTarget, animOptions) {
    if (animTarget instanceof Animation) {
      this._call(this.module._chart_anim_restore)(animTarget.id);
    } else {
      let anims = [];

      if (Array.isArray(animTarget)) {
        for (let target of animTarget)
          if (target.target !== undefined)
            anims.push({ target: target.target, options: target.options });
          else anims.push({ target: target, options: undefined });
      } else {
        anims.push({ target: animTarget, options: animOptions });
      }

      for (let anim of anims) this._setKeyframe(anim.target, anim.options);
    }
    this._setAnimation(animOptions);
  }

  _setKeyframe(animTarget, animOptions) {
    if (animTarget) {
      if (animTarget instanceof Snapshot) {
        this._call(this.module._chart_restore)(animTarget.id);
      } else {
        let obj = Object.assign({}, animTarget);

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
          this._container,
          this._propPrefix
        );
        this._setStyle(propsToObject(props, style, this._propPrefix));
        this._setConfig(Object.assign({}, obj.config));
      }
    }

    this._setAnimation(animOptions);

    this._call(this.module._chart_setKeyframe)();
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
        this._iterateObject(animOptions, (path, value) => {
          this._call(this.module._anim_setValue)(path, value);
        });
      } else {
        throw new Error("invalid animation option");
      }
    }
  }

  // keeping this one for backward compatibility
  get animation() {
    return new AnimControl(this);
  }

  version() {
    this._validateModule();
    let versionCStr = this.module._vizzu_version();
    let versionStr = this.module.UTF8ToString(versionCStr);
    return versionStr;
  }

  _start() {
    if (!this._started) {
      this._call(this.module._vizzu_poll)();
      this.render.updateFrame(false);

      this._pollInterval = setInterval(() => {
        this._call(this.module._vizzu_poll)();
      }, 10);

      this._updateInterval = setInterval(() => {
        this.render.updateFrame(false);
      }, 25);

      this._started = true;
    }
  }

  _getPointerPos(evt) {
    var rect = this.render.clientRect();
    return [evt.clientX - rect.left, evt.clientY - rect.top];
  }

  _toCString(str) {
    let len = str.length * 4 + 1;
    let buffer = this.module._malloc(len);
    this.module.stringToUTF8(str, buffer, len);
    return buffer;
  }

  _fromCString(str) {
    return this.module.UTF8ToString(str);
  }

  _init(module) {
    this.module = module;

    this.canvas = this._createCanvas();

    this.render = new Render();
    this.module.render = this.render;
    this._data = new Data(this);
    this.events = new Events(this);
    this.module.events = this.events;
    this._tooltip = new Tooltip(this);
    this.render.init(this._call(this.module._vizzu_update), this.canvas, false);
    this._objectRegistry = new ObjectRegistry(
      this._call(this.module._object_free)
    );
    this._call(this.module._vizzu_init)();
    this._call(this.module._vizzu_setLogging)(false);

    this._setupDOMEventHandlers(this.canvas);

    this._start();

    return this;
  }

  _createCanvas() {
    let canvas = null;
    let placeholder = this._container;

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

  _setupDOMEventHandlers(canvas) {
    this._resizeObserver = new ResizeObserver(() => {
      this.render.updateFrame(true);
    });

    this._resizeObserver.observe(canvas);

    this._resizeHandler = () => {
      this.render.updateFrame(true);
    };

    this._pointermoveHandler = (evt) => {
      let pos = this._getPointerPos(evt);
      this._call(this.module._vizzu_pointerMove)(evt.pointerId, pos[0], pos[1]);
    };

    this._pointerupHandler = (evt) => {
      let pos = this._getPointerPos(evt);
      this._call(this.module._vizzu_pointerUp)(evt.pointerId, pos[0], pos[1]);
    };

    this._pointerdownHandler = (evt) => {
      let pos = this._getPointerPos(evt);
      this._call(this.module._vizzu_pointerDown)(evt.pointerId, pos[0], pos[1]);
    };

    this._pointerleaveHandler = (evt) => {
      this._call(this.module._vizzu_pointerLeave)(evt.pointerId);
    };

    this._wheelHandler = (evt) => {
      this._call(this.module._vizzu_wheel)(evt.deltaY);
    };

    this._keydownHandler = (evt) => {
      let key = evt.keyCode <= 255 ? evt.keyCode : 0;
      const keys = [33, 34, 36, 35, 37, 39, 38, 40, 27, 9, 13, 46];
      for (let i = 0; i < keys.length; i++) {
        if (evt.key === keys[i]) {
          key = 256 + i;
        }
      }
      if (key !== 0) {
        this._call(this.module._vizzu_keyPress)(
          key,
          evt.ctrlKey,
          evt.altKey,
          evt.shiftKey
        );
      }
    };

    window.addEventListener("resize", this._resizeHandler);
    canvas.addEventListener("pointermove", this._pointermoveHandler);
    canvas.addEventListener("pointerup", this._pointerupHandler);
    canvas.addEventListener("pointerdown", this._pointerdownHandler);
    canvas.addEventListener("pointerleave", this._pointerleaveHandler);
    canvas.addEventListener("wheel", this._wheelHandler);
    document.addEventListener("keydown", this._keydownHandler);
  }

  detach() {
    this._resizeObserver.disconnect();
    clearInterval(this._pollInterval);
    clearInterval(this._updateInterval);
    window.removeEventListener("resize", this._resizeHandler);
    this.canvas.removeEventListener("pointermove", this._pointermoveHandler);
    this.canvas.removeEventListener("pointerup", this._pointerupHandler);
    this.canvas.removeEventListener("pointerdown", this._pointerdownHandler);
    this.canvas.removeEventListener("pointerleave", this._pointerleaveHandler);
    this.canvas.removeEventListener("wheel", this._wheelHandler);
    document.removeEventListener("keydown", this._keydownHandler);
  }
}
