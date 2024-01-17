import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Year',
					y: {
						set: 'Value 3 (+)',
						/* Making the chart elements fill the whole of
                    the y-axis as the default value is now 110% */
						range: {
							max: '6000000000'
						}
					},
					/* Add the dimension that we’ll use 
                in the next state without splitting
                the lines in this state. */
					size: 'Country'
				},
				title: 'Single Line Chart',
				geometry: 'line'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					color: 'Country',
					/* We don’t need this dimension here anymore
                since it’s already on the 'color' channel. */
					size: null
				},
				title: 'Drill down'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: {
						/* Setting back the y-axis range
                    to the default value. */
						range: {
							max: 'auto'
						}
					}
				},
				title: 'Line Chart II'
			}
		})
]

export default testSteps
