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
					x: ['Joy factors', 'Value 2 (+)'],
					y: 'Country',
					color: 'Joy factors',
					label: 'Value 2 (+)'
				},
				title: 'Range Bar Chart',
				legend: null
			},
			style: {
				plot: {
					marker: { colorPalette: '#f2b82dFF #00000000 #00000000' }
				}
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
