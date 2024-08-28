import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate(
			{
				data,
				config: {
					x: 'Joy factors',
					y: ['Country', 'Value 1 (+)'],
					color: {set: 'Country', title: 'offset test'}
				},
				style: {
					plot: {
						areaColor: '#FAFAFA'
					}
				}
			},
			0
		),
	(chart) =>
		chart.animate({
			config: {
				color: {title: '1em'}
			},
			style: {
				legend: {
					translateY: '1em'
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				color: {title: '2em'}
			},
			style: {
				legend: {
					translateY: '2em'
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				color: {title: '-2em'}
			},
			style: {
				legend: {
					translateY: '-2em'
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				color: {title: '100%'}
			},
			style: {
				legend: {
					translateY: '100%'
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				color: {title: '100%-2em'}
			},
			style: {
				legend: {
					translateY: '100%+-2em'
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				color: {title: null}
			},
			style: {
				legend: {
					translateY: null
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				color: {title: '17px'}
			},
			style: {
				legend: {
					translateY: '17px'
				}
			}
		})
]

export default testSteps
