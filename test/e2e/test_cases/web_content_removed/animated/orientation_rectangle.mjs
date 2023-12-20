import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Timeseries',
					y: ['Values 1', 'Categ. Parent'],
					color: 'Categ. Parent',
					label: 'Values 1'
				},
				title: 'Stacked Column Chart'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					/* Taking the dimension off to show the sum of
        the newly stacked elements. */
					x: 'Values 1',
					y: 'Categ. Parent'
				},
				title: 'Bar Chart'
			}
		})
]

export default testSteps
