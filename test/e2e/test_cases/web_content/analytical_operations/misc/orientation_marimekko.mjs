import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- change the orientation from 'horizontal' to 'vertical'`
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

				orientation: 'horizontal'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				x: { align: 'stretch' },
				y: { align: 'none' },
				orientation: 'vertical',
				label: 'Value 3 (+)'
			}
		})
]

export { description }
export default testSteps
