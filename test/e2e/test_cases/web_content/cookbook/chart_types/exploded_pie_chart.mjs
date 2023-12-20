import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) => {
		return chart.animate({
			data,
			config: {
				channels: {
					x: ['Joy factors', 'Value 2 (+)'],
					color: 'Joy factors',
					label: 'Value 2 (+)'
				},
				title: 'Pie Chart',
				coordSystem: 'polar'
			}
		})
	},
	(chart) => {
		class Shifter {
			constructor() {
				this.shifts = []
			}

			permanent(context, dx, dy) {
				context.translate(dx, dy)
				this.shifts.push([dx, dy])
			}

			temporal(context) {
				let shift = this.shifts.shift()
				if (!shift) shift = [0, 0]
				context.translate(...shift)
			}
		}

		let d = 0
		const dMax = 10
		let shifter

		chart.on('update', (event) => {
			const p = event.detail.progress
			d = Math.pow(p, 0.4) * dMax // easing
			shifter = new Shifter()
		})

		chart.on('plot-marker-label-draw', (event) => {
			const ctx = event.renderingContext
			shifter.temporal(ctx)
		})

		chart.on('plot-marker-draw', (event) => {
			const ctx = event.renderingContext
			const rect = event.detail.rect
			const angle = (rect.pos.x + rect.size.x / 2) * 2 * 3.14
			const dAngle = rect.size.x * 2 * 3.14
			const offset = d / 2 / Math.sin(dAngle / 2)
			const dx = +offset * Math.sin(angle)
			const dy = -offset * Math.cos(angle)
			shifter.permanent(ctx, dx, dy)
		})

		return chart.animate({
			config: {
				channels: {
					x: ['Joy factors', 'Value 2 (+)'],
					color: 'Joy factors',
					label: 'Value 2 (+)'
				},
				title: 'Exploded Pie Chart'
			},
			style: {
				plot: { overflow: 'visible' }
			}
		})
	}
]

export default testSteps
