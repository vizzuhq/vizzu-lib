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
					record.Country === 'Czechia' ||
					record.Country === 'Denmark' ||
					record.Country === 'Hungary'
			}),
			config: {
				channels: {
					y: 'Country',
					x: { attach: 'Value 5 (+/-)', range: { min: '0%', max: '110%' } },
					label: 'Value 5 (+/-)'
				},
				title: 'Bar Chart with (-) Nums'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
