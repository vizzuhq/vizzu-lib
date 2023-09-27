export default class Tooltip {
  meta = {
    name: 'tooltip'
  }

  listeners = {
    pointermove: this._mousemove.bind(this),
    pointeron: this._mouseon.bind(this)
  }

  constructor() {
    this.id = 0
    this.animating = false
    this.lastMarkerId = null
    this.lastMove = new Date()
  }

  register(chart) {
    this.chart = chart
  }

  enable(enabled) {
    if (!enabled) {
      this.id++
      setTimeout(() => {
        this._out(this.id)
      }, 200)
    }
  }

  _mousemove() {
    this.lastMove = new Date()
  }

  _mouseon(param) {
    this.id++
    const id = this.id
    if (!param.target || param.target.tagName !== 'plot-marker') {
      setTimeout(() => {
        this._out(id)
      }, 200)
    } else {
      setTimeout(() => {
        this._in(id, param.target.index)
      }, 0)
    }
  }

  _in(id, markerId) {
    if (this.id === id) {
      if (!this.animating) {
        this.lastMarkerId = markerId
        this.animating = true
        this.chart
          .animate({ config: { tooltip: markerId } }, this.lastMarkerId ? '100ms' : '250ms')
          .then(() => {
            this.animating = false
          })
      } else {
        setTimeout(() => {
          this._in(id, markerId)
        }, 100)
      }
    }
  }

  _out(id) {
    if (this.id === id) {
      const ellapsed = new Date() - this.lastMove
      if (!this.animating && ellapsed > 200) {
        this.lastMarkerId = null
        this.animating = true
        this.chart.animate({ config: { tooltip: null } }, '250ms').then(() => {
          this.animating = false
        })
      } else {
        setTimeout(() => {
          this._out(id)
        }, 200 - ellapsed)
      }
    }
  }
}
