import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Value 5 (+/-)'] },
					color: { set: ['Country'] },
					size: { set: ['Value 5 (+/-)'] }
				},
				title: 'Line Chart',
				geometry: 'line'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
