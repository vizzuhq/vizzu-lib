import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Value 2 (+)'] },
					noop: { set: ['Country'] },
					color: { set: ['Country'] }
				},
				title: 'Dotplot',
				geometry: 'circle',
				coordSystem: 'polar',
				legend: null
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Year', 'Value 2 (+)'] },
					y: { set: ['Country'] },
					color: { set: ['Country'] }
				},
				title: 'Area Chart Vertical',
				geometry: 'area',
				split: false
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Value 2 (+)'] },
					noop: { set: ['Country'] },
					color: { set: ['Country'] }
				},
				title: 'Dotplot',
				geometry: 'circle',
				legend: null
			}
		})
]

export default testSteps
