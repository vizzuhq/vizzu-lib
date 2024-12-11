import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Country', 'Value 2 (+)'], align: 'stretch' },
					color: 'Country'
				},
				title: 'Stacked Percentage Column'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					label: 'Value 2 (+)',
					y: {
						align: 'none',
						split: true
					}
				},
				title: 'ALign min & Split Components to See them Separately'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							fontSize: '0.6em'
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
