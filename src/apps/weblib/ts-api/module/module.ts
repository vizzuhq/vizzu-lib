import { ObjectRegistry } from './objregistry.js'
import { CEnv } from './cenv.js'
import { CData } from './cdata.js'
import { CChart } from './cchart.js'
import { CCanvas } from './ccanvas.js'

export class Module extends CEnv {
  constructor(wasm: CVizzu) {
    super(wasm, new ObjectRegistry(wasm._object_free))
    this._wasm.callback = this._callStatic(this._wasm._callback)
    this._wasm.renders = {}
  }

  registerRenderer(cCanvas: CCanvas, renderer: Renderer) {
    this._wasm.renders[cCanvas.getId()] = renderer
  }

  version() {
    return this._wasm.UTF8ToString(this._wasm._vizzu_version())
  }

  setLogging(enabled: boolean) {
    this._callStatic(this._wasm._vizzu_setLogging)(enabled)
  }

  getData(cChart: CChart) {
    return new CData(cChart.getId, this)
  }

  createChart() {
    return new CChart(this)
  }

  createCanvas() {
    return new CCanvas(this)
  }
}
