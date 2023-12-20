import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					label: 'Country_code',
					size: 'Value 2 (+)'
				},
				title: 'Treemap'
			}
		})
]

export default testSteps
