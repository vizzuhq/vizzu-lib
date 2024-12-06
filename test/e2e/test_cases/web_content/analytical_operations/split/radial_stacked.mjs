import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- set the Split parameter to true`
const testSteps = [
	(chart) => {
		const f = data_8.filter
		return chart.animate({
			data: Object.assign(data_8, {
				filter: (record) => f(record) && record.Year >= 15
			}),
			config: {
				channels: {
					x: ['Country', 'Value 2 (+)'],
					y: {
						set: 'Year',
						range: { min: '-3' }
					},
					color: 'Country'
				},

				coordSystem: 'polar'
			}
		})
	},

	(chart) =>
		chart.animate({
			config: {
				x: { split: true }
			}
		})
]

export { description }
export default testSteps
