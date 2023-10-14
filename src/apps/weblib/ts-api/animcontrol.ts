import { Anim } from './types/vizzu.js'

import { CChart, CAnimation } from './module/cchart.js'

export class AnimControl implements Anim.Control {
  _cChart: CChart

  constructor(cChart: CChart) {
    this._cChart = cChart
  }

  store(): CAnimation {
    return this._cChart.storeAnim()
  }

  seek(value: Anim.Position): this {
    const param = typeof value !== 'string' ? value.toString() : value
    this._animControl('seek', param)
    return this
  }

  // eslint-disable-next-line  accessor-pairs
  set speed(value: number) {
    const param = typeof value !== 'string' ? value.toString() : value
    this._animControl('setSpeed', param)
  }

  pause(): this {
    this._animControl('pause')
    return this
  }

  play(): this {
    this._animControl('play')
    return this
  }

  stop(): this {
    this._animControl('stop')
    return this
  }

  cancel(): this {
    this._animControl('cancel')
    return this
  }

  reverse(): this {
    this._animControl('reverse')
    return this
  }

  _animControl(command: string, param = ''): void {
    this._cChart.animControl(command, param)
  }
}
