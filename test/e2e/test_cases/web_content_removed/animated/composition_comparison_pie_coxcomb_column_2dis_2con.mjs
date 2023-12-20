import { data } from '../../../test_data/infinite_data.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Value 1', 'Joy factors'],
					color: 'Joy factors',
					label: 'Value 1'
				},
				title: 'Pie Chart',
				coordSystem: 'polar'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: ['Value 1', 'Joy factors', 'Region', 'Country code'],
						label: null
					}
				}
			},
			'500ms'
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Value 1', 'Joy factors', 'Region', 'Country code'],
					y: {
						set: 'Value 3',
						/* Setting the radius of the empty circle
                    in the centre. */
						range: { min: '-60%' }
					}
				},
				title: 'Coxcomb Chart'
			}
		})
]

export default testSteps
