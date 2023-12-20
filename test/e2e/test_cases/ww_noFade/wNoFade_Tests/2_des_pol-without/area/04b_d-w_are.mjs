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
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: null },
					y: { set: null },
					noop: { set: 'Year' },
					size: { set: ['Year', 'Value 2 (+)'] },
					color: { set: 'Joy factors' }
				},
				title: 'Change CoordSys',
				geometry: 'circle'
			}
		})
]

export default testSteps
