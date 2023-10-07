import { CObject } from './cenv.js'

export class Snapshot extends CObject {}

export class Animation extends CObject {}

export class CEvent extends CObject {
  preventDefault() {
    this._call(this._wasm._event_preventDefault)()
  }
}

export class CChart extends CObject {
  constructor(env) {
    const getId = env._get(env._wasm._vizzu_createChart)
    super(getId, env)
  }

  update(cCanvas, width, height, renderControl) {
    this._call(this._wasm._vizzu_update)(cCanvas.getId(), width, height, renderControl)
  }

  animate(callback) {
    const callbackPtr = this._wasm.addFunction((ok) => {
      callback(ok)
      this._wasm.removeFunction(callbackPtr)
    }, 'vi')
    this._call(this._wasm._chart_animate)(callbackPtr)
  }

  listConfigParams() {
    return this._listParams(this._wasm._chart_getList)
  }

  listStyleParams() {
    return this._listParams(this._wasm._style_getList)
  }

  getConfigParam(path) {
    const value = this._getParam(this._wasm._chart_getValue, path)
    // dirty workaround because config.channel.*.set returns already json instead of scalars
    return value.startsWith('[') || value.startsWith('{') ? JSON.parse(value) : value
  }

  getStyleParam(computed, path) {
    return this._getParam(this._wasm._style_getValue, path, computed)
  }

  setConfigParam(path, value) {
    this._setParam(this._wasm._chart_setValue, path, value)
  }

  setStyleParam(path, value) {
    this._setParam(this._wasm._style_setValue, path, value)
  }

  setAnimParam(path, value) {
    this._setParam(this._wasm._anim_setValue, path, value)
  }

  _listParams(lister) {
    const clistStr = this._call(lister)()
    const listStr = this._fromCString(clistStr)
    const list = JSON.parse(listStr)
    return list
  }

  _getParam(getter, path, ...args) {
    const cpath = this._toCString(path)
    try {
      const cvalue = this._call(getter)(cpath, ...args)
      return this._fromCString(cvalue)
    } finally {
      this._wasm._free(cpath)
    }
  }

  _setParam(setter, path, value) {
    const cpath = this._toCString(path)
    const cvalue = this._toCString(String(value).toString())
    try {
      this._call(setter)(cpath, cvalue)
    } finally {
      this._wasm._free(cvalue)
      this._wasm._free(cpath)
    }
  }

  animControl(command, param = '') {
    const ccommand = this._toCString(command)
    const cparam = this._toCString(param)
    try {
      this._call(this._wasm._anim_control)(ccommand, cparam)
    } finally {
      this._wasm._free(cparam)
      this._wasm._free(ccommand)
    }
  }

  storeSnapshot() {
    return new Snapshot(this._get(this._wasm._chart_store), this)
  }

  restoreSnapshot(snapshot) {
    this._call(this._wasm._chart_restore)(snapshot.getId())
  }

  storeAnim() {
    return new Animation(this._get(this._wasm._chart_anim_store), this)
  }

  restoreAnim(animation) {
    this._call(this._wasm._chart_anim_restore)(animation.getId())
  }

  setKeyframe() {
    this._call(this._wasm._chart_setKeyframe)()
  }

  addEventListener(eventName, func) {
    const wrappedFunc = (eventPtr, param) => {
      const eventObj = new CEvent(() => eventPtr, this)
      func(eventObj, JSON.parse(this._fromCString(param)))
    }
    const cfunc = this._wasm.addFunction(wrappedFunc, 'vii')
    const cname = this._toCString(eventName)
    try {
      this._call(this._wasm._addEventListener)(cname, cfunc)
    } finally {
      this._wasm._free(cname)
    }
    return cfunc
  }

  removeEventListener(eventName, cfunc) {
    const cname = this._toCString(eventName)
    try {
      this._call(this._wasm._removeEventListener)(cname, cfunc)
    } finally {
      this._wasm._free(cname)
    }
    this._wasm.removeFunction(cfunc)
  }

  getMarkerData(markerId) {
    const cStr = this._call(this._wasm._chart_markerData)(markerId)
    return JSON.parse(this._fromCString(cStr))
  }

  toCanvasCoords(point) {
    const ptr = this._call(this._wasm._chart_relToCanvasCoords)(point.x, point.y)
    return {
      x: this._wasm.getValue(ptr, 'double'),
      y: this._wasm.getValue(ptr + 8, 'double')
    }
  }

  toRelCoords(point) {
    const ptr = this._call(this._wasm._chart_canvasToRelCoords)(point.x, point.y)
    return {
      x: this._wasm.getValue(ptr, 'double'),
      y: this._wasm.getValue(ptr + 8, 'double')
    }
  }
}
