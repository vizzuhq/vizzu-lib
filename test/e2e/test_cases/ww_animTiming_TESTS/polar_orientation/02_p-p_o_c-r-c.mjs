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
					record.Country === 'Germany' ||
					record.Country === 'Denmark' ||
					record.Country === 'Estonia' ||
					record.Country === 'Greece' ||
					record.Country === 'Spain' ||
					record.Country === 'Finland' ||
					record.Country === 'France' ||
					record.Country === 'Croatia' ||
					record.Country === 'Hungary'
			}),

			config: {
				channels: {
					x: { set: ['Value 5 (+/-)', 'Year'] },
					y: { set: ['Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Stacked Coxcomb',
				coordSystem: 'polar',
				geometry: 'circle',
				legend: null
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Country', 'Value 2 (+)'] },
					y: { set: ['Year'] }
				},
				title: 'Stacked Coxcomb',
				geometry: 'rectangle',
				orientation: 'vertical',
				split: false
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Value 4 (+/-)'] },
					noop: { set: ['Year'] },
					y: { set: ['Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Stacked Coxcomb',
				geometry: 'circle',
				legend: null
			}
		})
]

export default testSteps
