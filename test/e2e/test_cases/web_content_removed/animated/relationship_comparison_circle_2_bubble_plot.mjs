import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Joy factors', 'Value 6 (+/-)'],
					y: 'Value 5 (+/-)',
					color: 'Joy factors',
					size: 'Value 2 (+)',
					label: 'Country_code'
				},
				title: 'Bubble Plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					/* With a dimension on the size channel Vizzu will
                stack the elements by the categories on the other channels
                on charts without coordinates. Here the Country code dimension is
                used to stack the bubbles by the dimension on the color channel. */
					size: ['Value 2 (+)', 'Country_code']
				},
				title: 'Stacked Bubble Chart'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					/* The stacking is eliminated when we remove the extra dimension
                from the size channel. */
					size: 'Value 2 (+)'
				},
				title: 'Bubble Chart'
			}
		})
]

export default testSteps
