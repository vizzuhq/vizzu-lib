import { data } from '../../../../test_data/chart_types_eu.mjs'
data.filter = (record) =>
	record.Country !== 'Germany' &&
	record.Country !== 'Italy' &&
	record.Country !== 'Hungary' &&
	record.Country !== 'Netherlands' &&
	record.Country !== 'Estonia' &&
	record.Country !== 'Belgium' &&
	record.Country !== 'Sweden' &&
	record.Country !== 'Poland' &&
	record.Country !== 'Malta' &&
	record.Country !== 'Romania' &&
	record.Country !== 'Greece' &&
	record.Country !== 'Slovakia' &&
	record.Country !== 'Ireland' &&
	record.Country !== 'Lithuania' &&
	record.Country !== 'Croatia' &&
	record.Country !== 'Slovenia' &&
	record.Country !== 'Portugal' &&
	record.Country !== 'Finland' &&
	record.Country !== 'United Kingdom'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '150%' } },
					x: { attach: ['Value 6 (+/-)'], range: { min: '0%', max: '150%' } },
					color: { attach: ['Country'] },
					size: { attach: ['Value 4 (+/-)'] },
					label: { attach: ['Value 5 (+/-)'] }
				},
				title: '1',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: '2'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: '3',
				coordSystem: 'polar'
			}
		})
]

export default testSteps
