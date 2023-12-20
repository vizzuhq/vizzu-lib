import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					color: 'Joy factors',
					label: 'Country_code',
					/* With a dimension on the size channel Vizzu will stack
                the elements by the categories on the other channels
                on charts without coordinates. Here the Country code dimension is
                used to stack the bubbles by the dimension on the color channel. */
					size: ['Country_code', 'Value 2 (+)']
				},
				title: 'Stacked Bubble Chart',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Country_code', 'Value 2 (+)'],
					y: 'Joy factors',
					label: null,
					/* The stacking is eliminated when we remove
                the extra dimension from the size channel. */
					size: null
				},
				title: 'Bar Chart',
				geometry: 'rectangle',
				orientation: 'vertical'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 2 (+)',
					label: 'Value 2 (+)'
				}
			}
		})
]

export default testSteps
