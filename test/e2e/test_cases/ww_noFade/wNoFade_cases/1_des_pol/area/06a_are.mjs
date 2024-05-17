import { data_14 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: 'Country_code',
					y: ['Joy factors', 'Value 2 (+)'],
					color: 'Joy factors'
				},
				title: 'Stacked Area Chart',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Country_code',
					y: 'Joy factors'
				},
				title: 'Remove Conti',
				align: 'stretch'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: 'Year',
						y: 'Joy factors'
					},
					title: 'Change Discrete'
				}
			},
			{
				duration: 1
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: ['Joy factors', 'Value 3 (+)']
				},
				title: 'Add Conti',
				align: 'min'
			}
		})
]

export default testSteps
