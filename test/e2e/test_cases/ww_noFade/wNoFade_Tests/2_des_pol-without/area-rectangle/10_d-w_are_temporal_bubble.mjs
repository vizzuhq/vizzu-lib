import { data_14 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Joy factors', 'Value 2 (+)'], align: 'center', split: true },
					color: 'Joy factors',
					noop: 'Year',
					size: 'Value 2 (+)'
				},
				title: 'Stacked Area Chart',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					noop: 'Year',
					size: ['Year', 'Value 2 (+)'],
					color: 'Joy factors'
				},
				title: 'Bubble',
				geometry: 'rectangle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					noop: null,
					size: ['Value 2 (+)'],
					color: 'Joy factors'
				},
				title: 'Bubble'
			}
		})
]

export default testSteps
