import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,
			config: {
				channels: {
					x: 'Value 2 (+)',
					y: 'Country',
					color: 'Country'
				},
				title: 'Treemap'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Value 2 (+)'],
					y: {
						set: 'Country',
						/* Setting the radius of
                    the empty circle in the centre. */
						range: {
							min: '-30%'
						}
					},
					size: null
				},
				title: 'Radial Chart',
				coordSystem: 'polar'
			},
			style: {
				plot: {
					paddingLeft: '25em'
				}
			}
		})
]

export default testSteps
