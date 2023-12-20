import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Joy factors', 'Value 2 (+)'],
					color: 'Joy factors',
					label: 'Value 2 (+)'
				},
				title: 'Pie Chart',
				coordSystem: 'polar'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					/* Setting the radius of
                the empty circle in the centre. */
					y: { range: { min: '-200%' } }
				},
				title: 'Donut Chart'
			}
		})
]

export default testSteps
