import { data } from '../../../../test_data/chart_types_eu.mjs'
import { Mouse } from '../../../../utils/mouse.mjs'

const testSteps = [
	(chart) => {
		class Throttle {
			constructor() {
				this.finished = true
				this.next = null
			}

			call(func) {
				if (!this.finished) {
					this.next = func
				} else {
					this.finished = false
					func().then(() => {
						this.finished = true
						if (this.next !== null) {
							const f = this.next
							this.next = null
							this.call(f)
						}
					})
				}
			}
		}

		class Window {
			constructor(chart, xmin, xmax, ymin, ymax) {
				this.chart = chart

				this.chart.on('pointerdown', (event) => {
					this.down(event.detail)
				})
				this.chart.on('pointerup', (event) => {
					this.up(event.detail)
				})
				this.chart.on('pointermove', (event) => {
					this.move(event.detail)
				})
				this.chart.on('draw-complete', (event) => {
					this.render(event.renderingContext)
				})

				this.x = { min: xmin, max: xmax }
				this.y = { min: ymin, max: ymax }
				this.xInit = { ...this.x }
				this.yInit = { ...this.y }
				this.throttle = new Throttle()

				this.reset()
			}

			zoomOut() {
				this.x = { ...this.xInit }
				this.y = { ...this.yInit }
				this.update()
				this.reset()
			}

			reset() {
				this.data0 = null
				this.data1 = null
			}

			isActive() {
				return this.data1 !== null
			}

			down(data) {
				this.data0 = data
			}

			move(data) {
				if (this.data0 !== null) this.data1 = data
			}

			up() {
				if (!this.isActive()) this.zoomOut()
				else this.applyZoom()
				this.reset()
			}

			posAt(range, factor) {
				return range.min + factor * (range.max - range.min)
			}

			applyZoomOnAxis(range, p0, p1) {
				const r = { ...range }
				range.min = this.posAt(r, Math.min(p0, p1))
				range.max = this.posAt(r, Math.max(p0, p1))
			}

			applyZoom() {
				const coordSystem = chart.feature.coordSystem
				const coords0 = coordSystem.toRelative(this.data0.position)
				const coords1 = coordSystem.toRelative(this.data1.position)
				this.applyZoomOnAxis(this.x, coords0.x, coords1.x)
				this.applyZoomOnAxis(this.y, coords0.y, coords1.y)
				this.update()
			}

			update() {
				this.throttle.call(() =>
					this.chart.animate(
						{ x: { range: this.x }, y: { range: this.y } },
						{ duration: '200ms', easing: 'linear' }
					)
				)
			}

			render(ctx) {
				if (this.isActive()) {
					ctx.lineWidth = 1
					ctx.strokeStyle = '#888888'
					ctx.beginPath()
					ctx.rect(
						this.data0.position.x,
						this.data0.position.y,
						this.data1.position.x - this.data0.position.x,
						this.data1.position.y - this.data0.position.y
					)
					ctx.stroke()
				}
			}
		}

		const win = new Window(chart, 0.5, 20.5, -500, 500)

		return chart.animate(
			{
				data,
				config: {
					x: { set: 'Year', range: win.x },
					y: { set: 'Value 5 (+/-)', range: win.y },
					title: 'Window Zoom',
					geometry: 'line'
				}
			},
			0
		)
	},
	(chart) => {
		return new Mouse(chart).down(100, 100).move(300, 250).up(300, 250).expectedAnimation()
	}
]

export default testSteps
