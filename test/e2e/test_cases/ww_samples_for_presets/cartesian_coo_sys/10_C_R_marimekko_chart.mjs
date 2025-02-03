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
					y: { set: ['Joy factors', 'Value 3 (+)'], align: 'stretch' },
					color: 'Joy factors',
					label: 'Value 2 (+)',
					lightness: 'Country'
				},
				title: 'Marimekko Chart',
				orientation: 'horizontal'
			}
		})
]

export default testSteps
