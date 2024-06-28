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
					record.Country === 'Denmark'
			}),
			config: {
				channels: {
					x: 'Country',
					y: 'Value 2 (+)',
					color: 'Joy factors'
				},
				title: 'Coxcomb, Nightingale, Rose',
				coordSystem: 'polar'
			},
			style: {
				plot: {
					marker: {
						fillOpacity: 0.5,
						borderWidth: 0
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
