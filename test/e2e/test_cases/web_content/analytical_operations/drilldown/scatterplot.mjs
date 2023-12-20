import { data_14 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- add a new Dimension to the no-operation (Noop) channel`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,

			config: {
				channels: {
					y: 'Value 4 (+/-)',
					x: 'Value 2 (+)',
					color: 'Country',
					label: 'Country'
				},

				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					noop: 'Year',
					label: null
				}
			}
		})
]

export { description }
export default testSteps
