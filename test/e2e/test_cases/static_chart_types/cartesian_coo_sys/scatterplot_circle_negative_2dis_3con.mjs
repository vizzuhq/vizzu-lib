import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: 'Value 5 (+/-)',
					x: 'Value 6 (+/-)',
					color: 'Country',
					size: 'Value 4 (+/-)',
					label: 'Value 5 (+/-)'
				},
				title: 'Scatterplot with (-) Nums',
				geometry: 'circle'
			}
		})
]

export default testSteps
