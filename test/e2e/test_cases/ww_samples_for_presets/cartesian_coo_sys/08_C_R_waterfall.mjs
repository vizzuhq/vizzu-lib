import { data } from '../../../test_data/chart_types_eu.mjs'

data.filter = (record) => record.Country === 'Belgium'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Year',
					y: ['Year', 'Value 5 (+/-)'],
					color: {
						attach: 'Value 5 (+/-)',
						range: {
							min: Number.MIN_SAFE_INTEGER,
							max: Number.MAX_SAFE_INTEGER
						}
					},
					label: 'Value 5 (+/-)'
				},
				title: 'Waterfall Chart',
				legend: null
			},
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
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
