import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) => {
		class LinearRegression {
			constructor() {
				this.reset()
			}

			reset() {
				this.xSum = 0
				this.ySum = 0
				this.xxSum = 0
				this.xySum = 0
				this.count = 0
				this.slope = null
				this.intercept = null
			}

			add(point) {
				this.xSum += point.x
				this.ySum += point.y
				this.xxSum += point.x * point.x
				this.xySum += point.x * point.y
				this.count++
			}

			calc() {
				this.slope =
					(this.count * this.xySum - this.xSum * this.ySum) /
					(this.count * this.xxSum - this.xSum * this.xSum)
				this.intercept = this.ySum / this.count - (this.slope * this.xSum) / this.count
			}

			at(x) {
				if (this.slope === null) this.calc()
				return x * this.slope + this.intercept
			}
		}

		const regression = new LinearRegression()

		function rectCenter(rect) {
			return {
				x: rect.pos.x + rect.size.x / 2,
				y: rect.pos.y + rect.size.y / 2
			}
		}

		chart.on('draw-begin', (event) => {
			regression.reset()
		})

		chart.on('plot-marker-draw', (event) => {
			regression.add(rectCenter(event.detail.rect))
		})

		chart.on('draw-complete', (event) => {
			const ctx = event.renderingContext
			ctx.strokeStyle = '#a0b0f0'
			ctx.beginPath()
			ctx.lineWidth = 2
			const coordSystem = chart.feature.coordSystem
			const p0 = coordSystem.toCanvas({ x: 0, y: regression.at(0) })
			const p1 = coordSystem.toCanvas({ x: 1, y: regression.at(1) })
			ctx.moveTo(p0.x, p0.y)
			ctx.lineTo(p1.x, p1.y)
			ctx.stroke()
		})

		return chart.animate({
			data,
			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Value 3 (+)',
					noop: 'Year',
					label: 'Year'
				},
				title: 'Scatterplot with (-) Nums',
				geometry: 'circle',
				coordSystem: 'cartesian',
				orientation: 'horizontal'
			}
		})
	}
]

export default testSteps
