/// <reference path="../cvizzu/cvizzu.d.ts" />
/// <reference path="../types/vizzu.d.ts" />

import { CObject, CEnv } from './cenv.js'
import { CProxy } from './cproxy.js'
import { CCanvas } from './ccanvas.js'

export class Snapshot extends CObject {}

export class CAnimation extends CObject {}

export class CEvent extends CObject {
  preventDefault() {
    this._call(this._wasm._event_preventDefault)()
  }
}

export class CConfig extends CProxy<Config.Chart> {
  constructor(cChart: CChart) {
    super(
      cChart.getId,
      cChart,
      cChart._wasm._chart_getList,
      cChart._wasm._chart_getValue,
      cChart._wasm._chart_setValue
    )
  }
}

export class CStyle extends CProxy<Styles.Chart> {
  constructor(cChart: CChart, computed: boolean) {
    super(
      cChart.getId,
      cChart,
      cChart._wasm._style_getList,
      (ptr, path) => cChart._wasm._style_getValue(ptr, path, computed),
      cChart._wasm._style_setValue
    )
  }
}

export class CAnimOptions extends CProxy<Anim.Options> {
  constructor(cChart: CChart) {
    super(
      cChart.getId,
      cChart,
      () => 0,
      () => 0,
      cChart._wasm._anim_setValue
    )
  }
}

export class CChart extends CObject {
  config: CConfig
  style: CStyle
  computedStyle: CStyle
  animOptions: CAnimOptions

  constructor(env: CEnv) {
    const getId = env._getStatic(env._wasm._vizzu_createChart)
    super(getId, env)

    this.config = new CConfig(this)
    this.style = new CStyle(this, false)
    this.computedStyle = new CStyle(this, true)
    this.animOptions = new CAnimOptions(this)
  }

  update(cCanvas: CCanvas, width: number, height: number, renderControl: number) {
    this._call(this._wasm._vizzu_update)(cCanvas.getId(), width, height, renderControl)
  }

  animate(callback: (ok: boolean) => void) {
    const callbackPtr = this._wasm.addFunction((ok: boolean) => {
      callback(ok)
      this._wasm.removeFunction(callbackPtr)
    }, 'vi')
    this._call(this._wasm._chart_animate)(callbackPtr)
  }

  animControl(command: string, param = '') {
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

  restoreSnapshot(snapshot: Snapshot) {
    this._call(this._wasm._chart_restore)(snapshot.getId())
  }

  storeAnim() {
    return new CAnimation(this._get(this._wasm._chart_anim_store), this)
  }

  restoreAnim(animation: CAnimation) {
    this._call(this._wasm._chart_anim_restore)(animation.getId())
  }

  setKeyframe() {
    this._call(this._wasm._chart_setKeyframe)()
  }

  addEventListener<T>(
    eventName: string,
    func: (event: CEvent, param: T) => void
  ): CFunction<(eventPtr: CEventPtr, param: CString) => void> {
    const wrappedFunc = (eventPtr: CEventPtr, param: CString) => {
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

  removeEventListener<T>(eventName: string, cfunc: CFunction<T>) {
    const cname = this._toCString(eventName)
    try {
      this._call(this._wasm._removeEventListener)(cname, cfunc)
    } finally {
      this._wasm._free(cname)
    }
    this._wasm.removeFunction(cfunc)
  }

  getMarkerData(markerId: number) {
    const cStr = this._call(this._wasm._chart_markerData)(markerId)
    return JSON.parse(this._fromCString(cStr))
  }

  toCanvasCoords(point: Geom.Point): Geom.Point {
    const ptr = this._call(this._wasm._chart_relToCanvasCoords)(point.x, point.y)
    return {
      x: this._wasm.getValue(ptr, 'double'),
      y: this._wasm.getValue(ptr + 8, 'double')
    }
  }

  toRelCoords(point: Geom.Point): Geom.Point {
    const ptr = this._call(this._wasm._chart_canvasToRelCoords)(point.x, point.y)
    return {
      x: this._wasm.getValue(ptr, 'double'),
      y: this._wasm.getValue(ptr + 8, 'double')
    }
  }
}
