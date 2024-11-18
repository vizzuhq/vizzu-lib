import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,
			config: {
				channels: {
					x: ['Country', 'Value 2 (+)'],
					y: {
						set: ['Joy factors', 'Value 3 (+)'],
						range: { max: '110%' }
					},
					color: 'Joy factors',
					label: ['Value 2 (+)']
				},
				title: 'Stacked Mekko Chart',
				orientation: 'horizontal'
			}
		})
]

export default testSteps
