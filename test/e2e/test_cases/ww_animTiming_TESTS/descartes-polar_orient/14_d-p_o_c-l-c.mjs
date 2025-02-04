import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Dotplot',
				geometry: 'circle',
				legend: null
			},
			style: {
				plot: {
					marker: {
						guides: {
							color: null,
							lineWidth: 0
						}
					}
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Value 2 (+)'] },
					noop: { set: ['Year'] },
					y: { set: ['Country'] },
					color: { set: ['Country'] }
				},
				title: 'Radial Line Vertical',
				geometry: 'line',
				coordSystem: 'polar'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Value 4 (+/-)'] },
					noop: { set: ['Year'] },
					y: { set: ['Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Dotplot',
				geometry: 'circle',
				coordSystem: 'cartesian',
				legend: null
			}
		})
]

export default testSteps
