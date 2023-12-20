import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: 'Value 5 (+/-)',
					x: 'Value 6 (+/-)',
					noop: ['Year']
				},
				title: 'Scatterplot with (-) Nums',
				legend: null,
				geometry: 'circle'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
