import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- set Split parameter from true to false`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Value 2 (+)', 'Country'], split: true },
					color: 'Country'
				},

				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				y: { split: false }
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps
