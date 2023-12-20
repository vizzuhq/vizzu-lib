import { data } from '../../../../test_data/chart_types_eu.mjs'

const description = `- set the range of the Y-axis to make empty circle in the center`
const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Joy factors', 'Value 2 (+)'],
					color: 'Joy factors',
					label: 'Value 2 (+)'
				},

				coordSystem: 'polar'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { range: { min: '-200%' } }
				}
			}
		})
]

export { description }
export default testSteps
