import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) =>
					(record.Country === 'Austria' ||
						record.Country === 'Belgium' ||
						record.Country === 'Bulgaria' ||
						record.Country === 'Cyprus' ||
						record.Country === 'Czechia' ||
						record.Country === 'Denmark') &&
					(record['Joy factors'] === 'Love' || record['Joy factors'] === 'Creativity')
			}),
			config: {
				channels: {
					x: 'Country',
					y: ['Joy factors', 'Value 2 (+)'],
					color: 'Joy factors'
				},
				title: 'Polar Range Column Chart',
				coordSystem: 'polar',
				legend: null
			},
			style: {
				plot: {
					marker: {
						rectangleSpacing: '0',
						colorPalette: '#f2b82dFF #00000000 #00000000'
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
