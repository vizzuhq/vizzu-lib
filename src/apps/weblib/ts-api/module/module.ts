import { CVizzu, Renderer } from '../cvizzu.types'

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
    this.setLogging(false)
  }

  registerRenderer(cCanvas: CCanvas, renderer: Renderer): void {
    this._wasm.renders[cCanvas.getId()] = renderer
  }

  version(): string {
    return this._wasm.UTF8ToString(this._wasm._vizzu_version())
  }

  setLogging(enabled: boolean): void {
    this._callStatic(this._wasm._vizzu_setLogging)(enabled)
  }

  getData(cChart: CChart): CData {
    return new CData(cChart.getId, this)
  }

  createChart(): CChart {
    return new CChart(this, this._getStatic(this._wasm._vizzu_createChart))
  }

  createCanvas(): CCanvas {
    return new CCanvas(this, this._getStatic(this._wasm._vizzu_createCanvas))
  }
}
