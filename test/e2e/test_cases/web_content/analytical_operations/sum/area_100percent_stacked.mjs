import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const title = '100% Stacked Area'
const description = `- set Align parameter from 'stretch' to 'min'`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Value 2 (+)', 'Country'], align: 'stretch' },
					color: 'Country'
				},

				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			y: {
				align: 'none'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { title }
export { description }
export default testSteps
