import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,
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
		}),

	(chart) =>
		chart.animate({
			config: {
				title: 'Marimekko with Other Orientation II',
				orientation: 'vertical',
				label: 'Value 3 (+)',
				x: { align: 'stretch' },
				y: { align: 'none' }
			}
		})
]

export default testSteps
