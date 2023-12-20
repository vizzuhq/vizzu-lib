import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Joy factors',
					y: { set: 'Value 2 (+)', range: { min: '-50%' } },
					label: 'Value 2 (+)'
				},
				title: 'Polar Column Chart (Yrange)',
				coordSystem: 'polar'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
