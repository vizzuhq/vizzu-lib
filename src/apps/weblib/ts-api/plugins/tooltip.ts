import { Snapshot } from '../module/cchart.js'
import { Element, Marker, MarkerLabel, PointerEvent } from '../events.js'
import { Plugin, type PluginHooks, type PrepareAnimationContext } from '../plugins.js'

import Vizzu from '../vizzu.js'

export class Tooltip implements Plugin {
	private _vizzu?: Vizzu
	private _id = 0
	private _animating = false
	private _lastMarkerId: string | null = null
	private _overedMarkerId: string | null = null
	private _lastMove = new Date().getTime()

	get hooks(): PluginHooks {
		return {
			prepareAnimation: (ctx: PrepareAnimationContext, next: () => void): void => {
				if (Array.isArray(ctx.target))
					ctx.target.forEach(({ target }) => {
						if (target instanceof Snapshot) return
						if (!target.config) target.config = {}
						if (!('tooltip' in target.config)) {
							target.config.tooltip = null
							this._id++
							this._lastMarkerId = null
						}
					})
				next()
			}
		}
	}

	meta = {
		name: 'tooltip',
		depends: ['pointerEvents']
	}

	listeners = {
		pointermove: this._mousemove.bind(this),
		pointerleave: this._mouseleave.bind(this)
	}

	register(vizzu: Vizzu): void {
		this._vizzu = vizzu
	}

	enable(enabled: boolean): void {
		if (!enabled && this._lastMarkerId !== null) {
			this._id++
			setTimeout(() => {
				this._out(this._id)
			}, 200)
		}
	}

	_mousemove(param: PointerEvent): void {
		this._lastMove = new Date().getTime()
		const actMarkerId = this._getMarkerId(param.target)
		if (actMarkerId !== this._overedMarkerId) {
			this._overedMarkerId = actMarkerId
			this._mouseon(param)
		}
	}

	_mouseleave(param: PointerEvent): void {
		this._lastMove = new Date().getTime()
		this._overedMarkerId = null
		this._mouseon(param)
	}

	_mouseon(param: PointerEvent): void {
		this._id++
		const id = this._id
		const markerId = this._getMarkerId(param.target)
		if (markerId !== null) {
			setTimeout(() => {
				this._in(id, markerId)
			}, 0)
		} else {
			setTimeout(() => {
				this._out(id)
			}, 200)
		}
	}

	_getMarkerId(target: Element | null): string | null {
		if (target && this._isMarker(target)) {
			return target.index
		} else if (target && this._isMarkerLabel(target)) {
			return target.parent.index
		} else {
			return null
		}
	}

	_isMarker(target: Element): target is Marker {
		return target.tagName === 'plot-marker'
	}

	_isMarkerLabel(target: Element): target is MarkerLabel {
		return target.tagName === 'plot-marker-label'
	}

	_in(id: number, markerId: string): void {
		if (this._id === id) {
			if (!this._animating) {
				this._lastMarkerId = markerId
				this._animating = true
				this._vizzu
					?.animate([
						{
							target: { config: { tooltip: markerId } },
							options: {
								duration: this._lastMarkerId ? '100ms' : '250ms'
							}
						}
					])
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
				this._vizzu
					?.animate([
						{ target: { config: { tooltip: null } }, options: { duration: '250ms' } }
					])
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
