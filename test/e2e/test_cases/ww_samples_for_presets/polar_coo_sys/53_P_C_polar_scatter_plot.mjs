import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { attach: 'Value 2 (+)', range: { max: '110%' } },
					x: 'Value 3 (+)',
					noop: 'Country'
				},
				title: 'Polar Scatter Plot',
				geometry: 'circle',
				coordSystem: 'polar'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
