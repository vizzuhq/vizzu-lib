import { data } from '../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
					x: { attach: ['Year'] },
					color: { attach: ['Country'] }
				},
				title: 'Line Chart with (-) Nums (2disc)',
				geometry: 'line'
			},
			style: {
				title: { backgroundColor: 'rgb(200,200,200 )' },
				legend: { backgroundColor: 'rgb(230,230,230 )' },
				plot: {
					backgroundColor: 'rgb(215,215,215 )',
					marker: {
						rectangleSpacing: 0.1
					}
				}
			}
		})
]

export default testSteps
