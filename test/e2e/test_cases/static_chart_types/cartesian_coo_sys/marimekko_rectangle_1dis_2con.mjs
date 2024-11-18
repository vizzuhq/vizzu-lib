import { data } from '../../../test_data/chart_types_eu.mjs'

data.filter = (record) =>
	record.Country === 'Bulgaria' ||
	record.Country === 'Germany' ||
	record.Country === 'Malta' ||
	record.Country === 'Lithuania' // Csak ezek látszódjanak.;

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: 'Value 1 (+)',
					x: ['Country', 'Value 2 (+)'],
					color: 'Country',
					label: ['Value 2 (+)']
				},
				title: 'Mekko chart'
			}
		})
]

export default testSteps
