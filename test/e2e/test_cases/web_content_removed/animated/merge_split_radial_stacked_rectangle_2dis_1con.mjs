import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) =>
					['11', '12', '13', '14', '15', '16', '17', '18', '19', '20'].includes(
						record.Year
					)
			}),
			config: {
				channels: {
					x: ['Country', 'Value 2 (+)'],
					y: {
						set: 'Year',
						/* Setting the radius of the empty circle
                    in the centre. */
						range: {
							min: '-3'
						}
					},
					color: 'Country'
				},
				title: 'Radial Bar Chart',
				coordSystem: 'polar'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				title: 'Split Radial Bar Chart',
				x: { split: true }
			}
		})
]

export default testSteps
