import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Value 6 (+/-)',
					y: 'Value 5 (+/-)',
					noop: 'Year',
					label: 'Year'
				},
				title: 'Scatterplot with (-) Nums',
				geometry: 'circle',
				coordSystem: 'cartesian'
			}
		})
]

export default testSteps
