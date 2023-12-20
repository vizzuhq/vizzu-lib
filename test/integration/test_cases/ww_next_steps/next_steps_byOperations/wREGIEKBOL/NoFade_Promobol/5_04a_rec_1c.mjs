import { data_6 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) => {
		const f = data_6.filter
		return chart.animate({
			data: Object.assign(data_6, {
				filter: (record) => f(record) && record.Year >= 10
			}),

			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country'
				},
				title: 'Stacked Column'
			}
		})
	},

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Joy factors',
					label: 'Value 2 (+)'
				},
				title: 'Change Main Category on the x Axis'
			}
		})
]

export default testSteps
