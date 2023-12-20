import { data } from '../../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Timeseries', 'Categ. Parent'],
					y: 'Values 1',
					label: 'Values 1'
				},
				title: 'Operations: Line - Drilled Down - X axis.',
				geometry: 'line'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Timeseries'
				},
				title: 'Operations: Line - Aggregated - X axis.'
			}
		})
]

export default testSteps
