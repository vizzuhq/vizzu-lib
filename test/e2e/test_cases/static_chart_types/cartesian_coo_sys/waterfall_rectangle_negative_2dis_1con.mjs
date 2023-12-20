import { data } from '../../../test_data/chart_types_eu.mjs'

data.filter = (record) => record.Country === 'Belgium'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: ['Year', 'Value 5 (+/-)'],
					x: 'Year',
					color: 'Country',
					label: 'Value 5 (+/-)'
				},
				title: 'Waterfall Chart with (-) Nums (Belgium)',
				legend: null
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top'
						}
					}
				}
			}
		})
]

export default testSteps
