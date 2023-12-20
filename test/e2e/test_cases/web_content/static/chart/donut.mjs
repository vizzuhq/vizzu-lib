import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Joy factors', 'Value 2 (+)'],
					/* Setting the radius of the empty circle
                in the centre. */
					y: { range: { min: '-200%' } },
					color: 'Joy factors',
					label: 'Value 2 (+)'
				},
				title: 'Donut Chart',
				coordSystem: 'polar'
			}
		})
]

export default testSteps
