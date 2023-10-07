export class AnimControl {
  constructor(chart) {
    this.chart = chart
  }

  get [Symbol.toStringTag]() {
    return 'AnimControl'
  }

  store() {
    return this.chart._cChart.storeAnim()
  }

  seek(value) {
    if (typeof value !== 'string') value = value.toString()
    this._animControl('seek', value)
    return this
  }

  // eslint-disable-next-line  accessor-pairs
  set speed(value) {
    if (typeof value !== 'string') value = value.toString()
    this._animControl('setSpeed', value)
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

  _animControl(command, param = '') {
    this.chart._cChart.animControl(command, param)
  }
}
