import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Country', 'Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Stacked Column Chart',
				geometry: 'rectangle',
				legend: null
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { set: ['Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Spider Line Fake',
				geometry: 'line',
				coordSystem: 'polar',
				split: false
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Country', 'Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Stacked Column Chart',
				geometry: 'rectangle',
				coordSystem: 'cartesian',
				legend: null
			}
		})
]

export default testSteps
