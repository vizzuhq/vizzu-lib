import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) => record.Country === 'Belgium'
			}),
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Value 5 (+/-)' },
					color: {
						set: 'Value 5 (+/-)',
						range: { min: '-45', max: '45' }
					},
					noop: { set: 'Country' },
					label: { set: 'Value 5 (+/-)' }
				},
				title: 'Column (+,-)',
				legend: 'color'
			},
			style: {
				plot: {
					marker: {
						colorGradient: [
							'#ac1727 0',
							'#e36c56 0.15',
							'#f4b096 0.35',
							'#d5d7d9 0.5',
							'#9fbffa 0.65',
							'#6389ec 0.85',
							'#3d51b8 1'
						].join(),
						label: { position: 'top' }
					}
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { set: ['Year', 'Value 5 (+/-)'] }
				},
				title: 'Waterfall / Sum?, Comparison?'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						y: { set: ['Value 5 (+/-)'] }
					},
					title: 'Column (+,-) / Goroup?'
				}
			},
			{
				delay: 2
			}
		)
]

export default testSteps
