import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: 'Year',
					y: ['Value 2 (+)', 'Country'],
					/* The noop channel splits the markers as all the other channels
                but will have no effect on the markers’ appearance. */
					noop: 'Country'
				},
				title: 'Column Chart'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: {
						/* Making the chart elements fill the whole of the y-axis
                    as the default value is now 110% */
						range: {
							max: '100%'
						},
						split: true
					},
					color: 'Country',
					noop: null
				},
				title: 'Split Column Chart'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: {
						/* Setting back the y-axis range to the default value. */
						range: {
							max: 'auto'
						},
						split: false
					}
				},
				title: 'Stacked Column Chart'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				title: '100% Column Chart',
				y: { align: 'stretch' }
			}
		})
]

export default testSteps
