import { data_6 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: {
				channels: {
					x: { set: 'Value 1 (+)' },
					y: { set: 'Country' },
					noop: { set: 'Year' },
					size: { set: null },
					color: { set: 'Country' }
				},
				title: '2 Dot plot / Distribution(?) - Continuous(?)',
				legend: 'color',
				geometry: 'circle',
				coordSystem: 'cartesian',
				orientation: 'vertical'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Value 1 (+)' },
						y: { set: 'Value 3 (+)' },
						noop: { set: 'Year' },
						size: { set: null },
						color: { set: 'Country' }
					},
					title: 'Scatter plot / Drill down(?), + Discrete(?)',
					legend: 'color',
					geometry: 'circle',
					coordSystem: 'cartesian'
				}
			},
			{
				delay: 1
			}
		),

	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
