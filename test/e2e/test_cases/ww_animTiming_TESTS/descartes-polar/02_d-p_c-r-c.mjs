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
				title: 'Spider Dotplot Fake',
				geometry: 'circle',
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
				title: 'Stacked Column Chart',
				geometry: 'rectangle',
				coordSystem: 'polar',
				//            orientation: 'horizontal',
				split: false
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Value 5 (+/-)'] },
					noop: { set: ['Year'] },
					y: { set: ['Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Spider Dotplot Fake',
				geometry: 'circle',
				coordSystem: 'cartesian',
				legend: null
			}
		})
]

export default testSteps
