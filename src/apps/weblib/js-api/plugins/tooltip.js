export class Tooltip {
  constructor() {
    this._id = 0
    this._animating = false
    this._lastMarkerId = null
    this._lastMove = new Date().getTime()
    this.meta = {
      name: 'tooltip',
      depends: ['pointerEvents']
    }
    this.listeners = {
      pointermove: this._mousemove.bind(this),
      pointeron: this._mouseon.bind(this)
    }
  }
  register(vizzu) {
    this._vizzu = vizzu
  }
  enable(enabled) {
    if (!enabled) {
      this._id++
      setTimeout(() => {
        this._out(this._id)
      }, 200)
    }
  }
  _mousemove() {
    this._lastMove = new Date().getTime()
  }
  _mouseon(param) {
    this._id++
    const id = this._id
    if (param.target && this._isMarker(param.target)) {
      const markerId = param.target.index
      setTimeout(() => {
        this._in(id, markerId)
      }, 0)
    } else {
      setTimeout(() => {
        this._out(id)
      }, 200)
    }
  }
  _isMarker(target) {
    return target.tagName === 'plot-marker'
  }
  _in(id, markerId) {
    if (this._id === id) {
      if (!this._animating) {
        this._lastMarkerId = markerId
        this._animating = true
        this._vizzu
          ?.animate([{ target: { config: { tooltip: markerId } } }], {
            duration: this._lastMarkerId ? '100ms' : '250ms'
          })
          .then(() => {
            this._animating = false
          })
      } else {
        setTimeout(() => {
          this._in(id, markerId)
        }, 100)
      }
    }
  }
  _out(id) {
    if (this._id === id) {
      const ellapsed = new Date().getTime() - this._lastMove
      if (!this._animating && ellapsed > 200) {
        this._lastMarkerId = null
        this._animating = true
        this._vizzu
          ?.animate([{ target: { config: { tooltip: null } } }], { duration: '250ms' })
          .then(() => {
            this._animating = false
          })
      } else {
        setTimeout(() => {
          this._out(id)
        }, 200 - ellapsed)
      }
    }
  }
}
