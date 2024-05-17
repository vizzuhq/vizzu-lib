import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Year'] },
					y: { attach: ['Value 2 (+)', 'Country_code'] },
					color: { attach: ['Country_code'] }
				},
				title: 'Area - Cartesian 2 Polar',
				geometry: 'area'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				coordSystem: 'polar'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				coordSystem: 'cartesian'
			}
		})
]

export default testSteps
