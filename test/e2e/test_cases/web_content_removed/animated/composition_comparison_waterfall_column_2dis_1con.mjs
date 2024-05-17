import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) => record.Country === 'Belgium'
			}),
			config: {
				channels: {
					x: 'Year',
					y: ['Year', 'Value 5 (+/-)'],
					color: {
						set: ['Value 5 (+/-)'],
						/* Setting the range of the colorGradient
                    symmetrically so that the color changes at 0 */
						range: {
							min: '-45',
							max: '45'
						}
					},
					noop: 'Country',
					label: 'Value 5 (+/-)'
				},
				title: 'Waterfall Chart',
				legend: 'color'
			},
			style: {
				plot: {
					marker: {
						/* Setting the colors used for the measure
                    on the color channel */
						colorGradient: [
							'#3d51b8 0',
							'#6389ec 0.15',
							'#9fbffa 0.35',
							'#d5d7d9 0.5',
							'#f4b096 0.65',
							'#e36c56 0.85',
							'#ac1727 1'
						].join(),
						label: {
							position: 'top'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 5 (+/-)'
				},
				title: 'Column Chart'
			}
		})
]

export default testSteps
