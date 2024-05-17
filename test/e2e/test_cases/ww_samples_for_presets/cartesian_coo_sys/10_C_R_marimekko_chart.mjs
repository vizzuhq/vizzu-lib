import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) =>
					record.Country === 'Bulgaria' ||
					record.Country === 'Germany' ||
					record.Country === 'Malta' ||
					record.Country === 'Lithuania'
			}),
			config: {
				channels: {
					x: ['Country', 'Value 2 (+)'],
					y: ['Joy factors', 'Value 3 (+)'],
					color: 'Joy factors',
					label: 'Country',
					lightness: 'Country'
				},
				title: 'Marimekko Chart',
				align: 'stretch',
				orientation: 'horizontal'
			}
		})
]

export default testSteps
