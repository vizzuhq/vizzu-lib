import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate(
			{
				data,
				config: {
					x: 'Joy factors',
					y: ['Country', 'Value 1 (+)'],
					color: 'Country'
				}
			},
			0
		),
	(chart) =>
		chart.animate({
			style: {
				legend: {
					translateY: '-2em'
				}
			}
		}),
	(chart) =>
		chart.animate({
			style: {
				legend: {
					translateY: '4em'
				}
			}
		}),
	(chart) =>
		chart.animate({
			style: {
				legend: {
					translateY: '100%'
				}
			}
		}),
	(chart) =>
		chart.animate({
			style: {
				legend: {
					translateY: '100%+-2em'
				}
			}
		}),
	(chart) =>
		chart.animate({
			style: {
				legend: {
					translateY: '1000000'
				}
			}
		}),
	(chart) =>
		chart.animate({
			style: {
				legend: {
					translateY: null
				}
			}
		}),
	(chart) =>
		chart.animate({
			style: {
				legend: {
					translateY: '10px'
				}
			}
		})
]

export default testSteps
