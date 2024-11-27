import { data_6 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: {
				channels: {
					x: ['Year', 'Joy factors'],
					y: { set: ['Value 3 (+)', 'Country'], align: 'center' },
					color: 'Country'
				},
				title: 'Stacked Streamgraph',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: {
						/* Making the chart elements fill the whole of
                        the y-axis as the default value is now 110% */
						range: {
							max: '100%'
						},
						split: true,
						align: 'none'
					}
				},
				title: 'Split Area Chart'
			}
		})
]

export default testSteps
