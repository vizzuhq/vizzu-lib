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
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Value 2 (+)' },
					color: null
				},
				title: 'Remove Discrete & Color',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Value 3 (+)' },
					color: null
				},
				title: 'Change Continuous'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: ['Joy factors', 'Value 3 (+)'] },
						color: null,
						noop: { set: 'Year' },
						size: { set: 'Value 3 (+)' }
					},
					title: 'Add Discrete'
				}
			},
			{
				duration: 0
			}
		),

	(chart) =>
		chart.animate(
			{
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
			},
			{
				geometry: {
					duration: 1
				},
				x: {
					delay: 0.5
				}
			}
		),

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
