import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Country', 'Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Stacked Polar Area',
				coordSystem: 'polar',
				geometry: 'area',
				legend: null
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Value 2 (+)', 'Country'] },
					color: { set: ['Country'] }
				},
				title: 'Stacked Coxcomb',
				geometry: 'rectangle',
				//            orientation: 'horizontal',
				split: false
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Country', 'Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Stacked Polar Area',
				geometry: 'area',
				legend: null
			}
		})
]

export default testSteps
