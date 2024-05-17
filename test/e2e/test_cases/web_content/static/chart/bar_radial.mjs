import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Value 2 (+)',
					y: {
						set: ['Joy factors'],
						/* Setting the radius of the empty circle
                    in the centre. */
						range: { min: '-30%' }
					},
					color: 'Joy factors',
					label: 'Value 2 (+)'
				},
				title: 'Radial Bar Chart',
				coordSystem: 'polar'
			}
		})
]

export default testSteps
