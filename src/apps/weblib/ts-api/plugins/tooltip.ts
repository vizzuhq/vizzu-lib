import { Events, Plugins } from '../types/vizzu.js'

import Vizzu from '../vizzu.js'

export class Tooltip implements Plugins.Plugin {
  private _vizzu?: Vizzu
  private _id = 0
  private _animating = false
  private _lastMarkerId: number | null = null
  private _lastMove = new Date().getTime()

  meta = {
    name: 'tooltip',
    depends: ['pointerEvents']
  }

  listeners = {
    pointermove: this._mousemove.bind(this),
    pointeron: this._mouseon.bind(this)
  }

  register(vizzu: Vizzu): void {
    this._vizzu = vizzu
  }

  enable(enabled: boolean): void {
    if (!enabled) {
      this._id++
      setTimeout(() => {
        this._out(this._id)
      }, 200)
    }
  }

  _mousemove(): void {
    this._lastMove = new Date().getTime()
  }

  _mouseon(param: Events.PointerEvent): void {
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

  _isMarker(target: Events.Element): target is Events.Marker {
    return target.tagName === 'plot-marker'
  }

  _in(id: number, markerId: number): void {
    if (this._id === id) {
      if (!this._animating) {
        this._lastMarkerId = markerId
        this._animating = true
        this._vizzu
          ?.animate(
            [{ target: { config: { tooltip: markerId } } }],
            this._lastMarkerId ? '100ms' : '250ms'
          )
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

  _out(id: number): void {
    if (this._id === id) {
      const ellapsed = new Date().getTime() - this._lastMove
      if (!this._animating && ellapsed > 200) {
        this._lastMarkerId = null
        this._animating = true
        this._vizzu?.animate([{ target: { config: { tooltip: null } } }], '250ms').then(() => {
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
