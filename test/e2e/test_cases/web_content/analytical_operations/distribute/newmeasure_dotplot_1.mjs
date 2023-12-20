import { data_1974_1990 } from '../../../../test_data/IMDB_data.mjs'

const description = `- move the Dimension from Y-axis to the no-operation (Noop) channel
- add a Measure to the Y-axis 
- set the Split parameter to true`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_1974_1990,
			config: {
				channels: {
					x: 'Year',
					y: { set: 'Index', range: { max: '105%' } }
				},

				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { set: 'IMDb Rating', range: { max: '105%' } },
					noop: 'Index'
				},

				orientation: 'vertical',
				split: true
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps
