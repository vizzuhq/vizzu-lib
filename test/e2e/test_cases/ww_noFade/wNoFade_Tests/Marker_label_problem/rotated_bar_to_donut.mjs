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
					y: { set: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
					x: 'Country',
					color: 'Country',
					label: 'Value 5 (+/-)'
				},
				title: 'Rectangle Marker Label Positions - Center',
				angle: Math.PI / 2
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { range: { min: 'auto', max: 'auto' } },
					x: ['Country', 'Value 2 (+)'],
					label: { attach: ['Country'] }
				},
				title: 'Polar Coordinate',
				angle: (Math.PI * 3) / 2,
				coordSystem: 'polar'
			}
		})
]

export default testSteps
