import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					lightness: { attach: ['Country'] },
					color: { attach: ['Value 5 (+/-)'] },
					size: { attach: ['Value 4 (+/-)'] },
					label: { attach: ['Country'] }
				},
				title: 'Filter on',
				legend: 'color',
				geometry: 'circle'
			},
			style: {
				plot: {
					marker: {
						colorGradient:
							'#3d51b8 0, #6389ec 0.15, #9fbffa 0.35, #d5d7d9 0.5, #f4b096 0.65, #e36c56 0.85, #ac1727 1'
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) =>
					record.Country !== 'Germany' &&
					record.Country !== 'Italy' &&
					record.Country !== 'Hungary' &&
					record.Country !== 'Denmark' &&
					record.Country !== 'Bulgaria' &&
					record.Country !== 'Lithuania' &&
					record.Country !== 'Croatia' &&
					record.Country !== 'Slovenia' &&
					record.Country !== 'Netherlands'
			},
			config: {
				title: 'Filtered'
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: null
			},
			config: {
				title: 'Filter off'
			}
		})
]

export default testSteps
