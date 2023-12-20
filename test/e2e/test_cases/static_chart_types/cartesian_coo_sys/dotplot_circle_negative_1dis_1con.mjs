import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Year', 'Value 5 (+/-)']
				},
				title: 'DotPlot with (-) Nums',
				geometry: 'circle'
			}
		})
]

export default testSteps
