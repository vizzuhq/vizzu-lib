import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Year',
					y: { set: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } }
				},
				title: 'Area Chart with (-) Nums',
				align: 'none',
				geometry: 'area'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
