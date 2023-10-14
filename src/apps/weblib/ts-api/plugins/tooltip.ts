import { Events, Plugins } from '../types/vizzu.js'

import Vizzu from '../vizzu.js'

export class Tooltip implements Plugins.Plugin {
  vizzu?: Vizzu
  id = 0
  animating = false
  lastMarkerId: number | null = null
  lastMove = new Date().getTime()

  meta = {
    name: 'tooltip',
    depends: ['pointerEvents']
  }

  listeners = {
    pointermove: this._mousemove.bind(this),
    pointeron: this._mouseon.bind(this)
  }

  register(vizzu: Vizzu): void {
    this.vizzu = vizzu
  }

  enable(enabled: boolean): void {
    if (!enabled) {
      this.id++
      setTimeout(() => {
        this._out(this.id)
      }, 200)
    }
  }

  _mousemove(): void {
    this.lastMove = new Date().getTime()
  }

  _mouseon(param: Events.PointerEvent): void {
    this.id++
    const id = this.id
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

  _isMarker(target: Events.Element): target is Events.Marker {
    return target.tagName === 'plot-marker'
  }

  _in(id: number, markerId: number): void {
    if (this.id === id) {
      if (!this.animating) {
        this.lastMarkerId = markerId
        this.animating = true
        this.vizzu
          ?.animate(
            [{ target: { config: { tooltip: markerId } } }],
            this.lastMarkerId ? '100ms' : '250ms'
          )
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

  _out(id: number): void {
    if (this.id === id) {
      const ellapsed = new Date().getTime() - this.lastMove
      if (!this.animating && ellapsed > 200) {
        this.lastMarkerId = null
        this.animating = true
        this.vizzu?.animate([{ target: { config: { tooltip: null } } }], '250ms').then(() => {
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
