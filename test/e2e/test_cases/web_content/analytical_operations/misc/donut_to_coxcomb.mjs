import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- add a new Measure to the Y-axis
- change the range of the Y-axis to disappear empty circle from the center`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: ['Value 2 (+)', 'Country'],
					y: { range: { min: '-200%' } },
					color: 'Country',
					label: 'Value 2 (+)'
				},

				coordSystem: 'polar'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Value 2 (+)', 'Country'],
					y: { set: 'Value 3 (+)', range: { min: '0' } }
				}
			}
		})
]

export { description }
export default testSteps
