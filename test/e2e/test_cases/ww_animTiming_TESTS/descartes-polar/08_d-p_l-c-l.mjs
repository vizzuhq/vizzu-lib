import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Stacked Area Chart',
				geometry: 'line',
				legend: null
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Year', 'Value 5 (+/-)'] },
					y: { set: ['Value 2 (+)', 'Country'] },
					color: { set: ['Country'] }
				},
				title: 'Scatter plot',
				geometry: 'circle',
				coordSystem: 'polar',
				orientation: 'horizontal'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Stacked Area Chart',
				geometry: 'line',
				coordSystem: 'cartesian',
				legend: null
			}
		})
]

export default testSteps
