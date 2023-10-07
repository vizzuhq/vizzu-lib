import { ObjectRegistry } from './objregistry.js'
import { CEnv, CObject } from './cenv.js'
import { CData } from './cdata.js'
import { CChart } from './cchart.js'

class CCanvas extends CObject {
  constructor(env) {
    const getId = env._get(env._wasm._vizzu_createCanvas)
    super(getId, env)
  }
}

export class Module extends CEnv {
  constructor(wasm) {
    super(wasm, new ObjectRegistry(wasm._object_free))
    this._wasm.callback = this._call(this._wasm._callback)
    this._wasm.renders = {}
  }

  registerRenderer(cCanvas, renderer) {
    this._wasm.renders[cCanvas.getId()] = renderer
  }

  version() {
    return this._wasm.UTF8ToString(this._wasm._vizzu_version())
  }

  setLogging(enabled) {
    this._call(this._wasm._vizzu_setLogging)(enabled)
  }

  getData(cChart) {
    return new CData(cChart.getId, this)
  }

  createChart() {
    return new CChart(this)
  }

  createCanvas() {
    return new CCanvas(this)
  }
}
