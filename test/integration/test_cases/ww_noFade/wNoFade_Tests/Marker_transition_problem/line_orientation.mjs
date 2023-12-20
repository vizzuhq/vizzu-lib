import { data } from '../../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Timeseries',
					y: 'Values 1',
					color: 'Categ. Parent',
					label: 'Values 1'
				},
				title: 'Operations: Line - Orientation Change - No:1.',
				geometry: 'line',
				orientation: 'horizontal'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Values 1',
					y: ['Categ. Parent', 'Values 1'],
					color: 'Timeseries'
				},
				title: 'Operations: Line - Orientation Change - No:2',
				orientation: 'vertical'
			}
		})
]

export default testSteps
