import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate(
			{
				data: data_6,

				config: {
					channels: {
						x: ['Value 2 (+)', 'Country'],
						y: { range: { min: '-200%' } },
						color: 'Country',
						label: 'Value 2 (+)'
					},
					title: 'Donat Chart',
					coordSystem: 'polar'
				}
			}
			//    {regroupStrategy: 'fade'}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Value 2 (+)', 'Country'],
					y: { set: 'Value 3 (+)', range: { min: '0' } }
					//                label: null
				},
				title: 'Coxcomb to Show One More Measure '
			}
		})
]

export default testSteps
