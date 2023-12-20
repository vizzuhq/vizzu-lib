import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					color: 'Joy factors',
					size: 'Value 2 (+)',
					label: 'Joy factors'
				},
				title: 'Treemap'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 2 (+)',
					y: {
						set: 'Joy factors',
						/* Setting the radius of
                    the empty circle in the centre. */
						range: {
							min: '-30%'
						}
					},
					size: null,
					label: 'Value 2 (+)'
				},
				title: 'Radial Chart',
				coordSystem: 'polar'
			}
		})
]

export default testSteps
