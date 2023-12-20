import { LastAnimation } from './lastanimation.mjs'

export class Mouse {
	constructor(chart) {
		chart.feature(new LastAnimation())
		this._chart = chart
	}

	expectedAnimation() {
		return this._chart.feature.lastAnimation.last()
	}

	click(x, y) {
		return this.down(x, y).up(x, y)
	}

	down(x, y) {
		this._dispatch('pointerdown', x, y)
		return this
	}

	move(x, y) {
		this._dispatch('pointermove', x, y)
		return this
	}

	up(x, y) {
		this._dispatch('pointerup', x, y)
		return this
	}

	wheel(delta) {
		const event = new WheelEvent('wheel', {
			bubbles: true,
			cancelable: true,
			deltaY: delta
		})
		this._chart.feature.htmlCanvas.element.dispatchEvent(event)
		return this
	}

	_dispatch(type, x, y) {
		const event = new PointerEvent(type, {
			bubbles: true,
			cancelable: true,
			pointerId: 1,
			clientX: x,
			clientY: y
		})
		this._chart.feature.htmlCanvas.element.dispatchEvent(event)
	}
}
