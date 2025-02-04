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
				orientation: 'vertical'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Value 4 (+/-)'] },
					y: { set: ['Value 2 (+)'] },
					noop: { set: ['Year'] },
					color: { set: ['Country'] }
				},
				title: 'Dotplot',
				geometry: 'circle',
				legend: null
			}
		})
]

export default testSteps
