import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) =>
					record.Country === 'Austria' ||
					record.Country === 'Belgium' ||
					record.Country === 'Bulgaria'
			}),
			config: {
				channels: {
					x: ['Joy factors', 'Value 2 (+)'],
					y: { attach: ['Country'], range: { min: '-50%' } },
					color: 'Joy factors',
					label: 'Value 2 (+)'
				},
				title: 'Nested (Multi-level) Donut Chart',
				align: 'stretch',
				coordSystem: 'polar'
			},
			style: {
				plot: {
					marker: {
						rectangleSpacing: '0',
						borderWidth: 1,
						borderOpacity: 0
					}
				}
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
