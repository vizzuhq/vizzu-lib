import { data } from '../../../test_data/chart_types_eu.mjs'

data.filter = (record) =>
	record.Country === 'Bulgaria' ||
	record.Country === 'Germany' ||
	record.Country === 'Malta' ||
	record.Country === 'Lithuania'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: ['Joy factors', 'Value 2 (+)'],
					x: ['Country', 'Value 1 (+)'],
					color: 'Joy factors',
					label: 'Value 1 (+)'
				},
				title: 'Stacked Mekko chart'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
