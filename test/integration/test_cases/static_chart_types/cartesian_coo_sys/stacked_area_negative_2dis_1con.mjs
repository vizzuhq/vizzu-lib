import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: ['Country', 'Value 5 (+/-)'],
					x: 'Year',
					color: 'Country'
				},
				title: 'Stacked Area Chart with (-) Nums',
				geometry: 'area'
			}
		})
]

export default testSteps
