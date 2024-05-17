import { data } from '../../../test_data/correlogram_data.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Alphabet 1',
					y: { set: 'Alphabet 2', range: { max: '110%' } },
					color: 'Frequency',
					size: 'Frequency'
				},
				title: 'Correlogram',
				geometry: 'circle'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
