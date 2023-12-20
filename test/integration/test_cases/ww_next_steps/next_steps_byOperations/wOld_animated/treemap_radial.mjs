import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					color: 'Joy factors',
					size: ['Country', 'Value 2 (+)'],
					lightness: 'Value 3 (+)'
				},
				title: 'Treemap'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Country', 'Value 2 (+)'],
					y: {
						set: 'Joy factors',
						/* Setting the radius of
                    the empty circle in the centre. */
						range: {
							min: '-100%'
						}
					},
					size: null
				},
				title: 'Radial Chart',
				coordSystem: 'polar'
			}
		})
]

export default testSteps
