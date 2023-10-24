import { CObject } from './cenv.js'
import { CProxy } from './cproxy.js'
/** Stored Chart object. */
export class Snapshot extends CObject {}
export class CEvent extends CObject {
  preventDefault() {
    this._call(this._wasm._event_preventDefault)()
  }
}
class CConfig extends CProxy {}
class CStyle extends CProxy {}
class CAnimOptions extends CProxy {}
export class CChart extends CObject {
  constructor(env, getId) {
    super(getId, env)
    this.config = this._makeConfig()
    this.style = this._makeStyle(false)
    this.computedStyle = this._makeStyle(true)
    this.animOptions = this._makeAnimOptions()
  }
  update(cCanvas, width, height, renderControl) {
    this._cCanvas = cCanvas
    this._call(this._wasm._vizzu_update)(cCanvas.getId(), width, height, renderControl)
  }
  animate(callback) {
    const callbackPtr = this._wasm.addFunction((ok) => {
      callback(ok)
      this._wasm.removeFunction(callbackPtr)
    }, 'vi')
    this._call(this._wasm._chart_animate)(callbackPtr)
  }
  storeSnapshot() {
    return new Snapshot(this._get(this._wasm._chart_store), this)
  }
  restoreSnapshot(snapshot) {
    this._call(this._wasm._chart_restore)(snapshot.getId())
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
  pointerdown(pointerId, x, y) {
    if (!this._cCanvas) return
    this._call(this._wasm._vizzu_pointerDown)(this._cCanvas.getId(), pointerId, x, y)
  }
  pointermove(pointerId, x, y) {
    if (!this._cCanvas) return
    this._call(this._wasm._vizzu_pointerMove)(this._cCanvas.getId(), pointerId, x, y)
  }
  pointerup(pointerId, x, y) {
    if (!this._cCanvas) return
    this._call(this._wasm._vizzu_pointerUp)(this._cCanvas.getId(), pointerId, x, y)
  }
  pointerleave(pointerId) {
    if (!this._cCanvas) return
    this._call(this._wasm._vizzu_pointerLeave)(this._cCanvas.getId(), pointerId)
  }
  wheel(delta) {
    if (!this._cCanvas) return
    this._call(this._wasm._vizzu_wheel)(this._cCanvas.getId(), delta)
  }
  _makeConfig() {
    return new CConfig(
      this.getId,
      this,
      this._wasm._chart_getList,
      this._wasm._chart_getValue,
      this._wasm._chart_setValue
    )
  }
  _makeStyle(computed) {
    return new CStyle(
      this.getId,
      this,
      this._wasm._style_getList,
      (ptr, path) => this._wasm._style_getValue(ptr, path, computed),
      this._wasm._style_setValue
    )
  }
  _makeAnimOptions() {
    return new CAnimOptions(
      this.getId,
      this,
      () => 0,
      () => 0,
      this._wasm._anim_setValue
    )
  }
}
