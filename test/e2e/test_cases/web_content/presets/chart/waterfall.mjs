import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) => record.Country === 'Belgium'
			}),
			config: chart.constructor.presets.waterfall({
				x: 'Year',
				y: 'Value 5 (+/-)',
				title: 'Waterfall Chart'
			}),
			style: {
				plot: {
					marker: {
						colorGradient: [
							'#ff001b 0',
							'#ff001b 0.5',
							'#7e79e8 0.5',
							'#7e79e8 1'
						].join(),
						label: {
							position: 'top'
						}
					}
				}
			}
		})
]

export default testSteps
