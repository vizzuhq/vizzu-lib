import { data_6 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					color: 'Country',
					size: ['Year', 'Value 2 (+)'],
					label: 'Value 2 (+)'
				},
				title: 'Bubble Chart',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: { set: 'Country', range: { max: '110%' } },
					label: null
				},
				title: 'Then Show the Time Distribution of The Values'
			}
		})
]

export default testSteps
