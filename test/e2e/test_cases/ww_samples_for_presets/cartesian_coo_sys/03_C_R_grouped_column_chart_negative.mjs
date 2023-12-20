import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) =>
					record.Country === 'Austria' ||
					record.Country === 'Belgium' ||
					record.Country === 'Bulgaria' ||
					record.Country === 'Cyprus' ||
					record.Country === 'Denmark' ||
					record.Country === 'Hungary'
			}),
			config: {
				channels: {
					y: { set: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
					x: ['Joy factors', 'Country'],
					color: 'Joy factors',
					label: 'Value 5 (+/-)'
				},
				title: 'Grouped Column Chart with (-) Nums'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
