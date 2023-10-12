/// <reference path="./types/vizzu.d.ts" />

import { CChart, CAnimation } from './module/cchart.js'

export class AnimControl implements Anim.Control {
  _cChart: CChart

  constructor(cChart: CChart) {
    this._cChart = cChart
  }

  store(): CAnimation {
    return this._cChart.storeAnim()
  }

  seek(value: Anim.Position) {
    const param = typeof value !== 'string' ? value.toString() : value
    this._animControl('seek', param)
    return this
  }

  // eslint-disable-next-line  accessor-pairs
  set speed(value: number) {
    const param = typeof value !== 'string' ? value.toString() : value
    this._animControl('setSpeed', param)
  }

  pause() {
    this._animControl('pause')
    return this
  }

  play() {
    this._animControl('play')
    return this
  }

  stop() {
    this._animControl('stop')
    return this
  }

  cancel() {
    this._animControl('cancel')
    return this
  }

  reverse() {
    this._animControl('reverse')
    return this
  }

  _animControl(command: string, param = '') {
    this._cChart.animControl(command, param)
  }
}
