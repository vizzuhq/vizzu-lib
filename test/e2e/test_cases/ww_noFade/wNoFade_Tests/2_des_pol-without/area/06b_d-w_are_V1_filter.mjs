import { data_14 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: ['Country', 'Value 2 (+)'] },
					color: { set: 'Country' }
				},
				title: 'Stacked Area Chart',
				geometry: 'area',
				orientation: 'horizontal'
			}
		}),

	(chart) =>
		chart.animate({
			data: {
				filter: (record) =>
					record['Joy factors'] !== 'Creativity' && record['Joy factors'] !== 'Love'
			},
			config: {
				channels: {
					x: null,
					y: null,
					color: { set: 'Joy factors' },
					noop: { set: 'Year' },
					size: { set: ['Year', 'Value 3 (+)'] }
				},
				title: 'Change Geoms & CoordSys',
				geometry: 'circle'
			}
		})
]

export default testSteps
