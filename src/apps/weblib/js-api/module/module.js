import { ObjectRegistry } from './objregistry.js'
import { CEnv } from './cenv.js'
import { CData } from './cdata.js'
import { CChart } from './cchart.js'
import { CCanvas } from './ccanvas.js'
import { CAnimControl } from './canimctrl.js'
export class Module extends CEnv {
  constructor(wasm) {
    super(wasm, new ObjectRegistry(wasm._object_free))
    this._wasm.callback = this._callStatic(this._wasm._callback)
    this._wasm.renders = {}
    this.setLogging(false)
  }
  registerRenderer(cCanvas, renderer) {
    this._wasm.renders[cCanvas.getId()] = renderer
  }
  version() {
    return this._wasm.UTF8ToString(this._wasm._vizzu_version())
  }
  setLogging(enabled) {
    this._callStatic(this._wasm._vizzu_setLogging)(enabled)
  }
  getData(cChart) {
    return new CData(cChart.getId, this)
  }
  getAnimControl(cChart) {
    return new CAnimControl(cChart.getId, this)
  }
  createChart() {
    return new CChart(this, this._getStatic(this._wasm._vizzu_createChart))
  }
  createCanvas() {
    return new CCanvas(this, this._getStatic(this._wasm._vizzu_createCanvas))
  }
}
