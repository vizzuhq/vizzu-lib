import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					color: 'Country_code',
					size: ['Value 2 (+)', 'Country_code'],
					label: 'Country_code'
				},
				title: 'Treemap'
			}
		})
]

export default testSteps
