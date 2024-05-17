import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: {
						attach: ['Value 5 (+/-)', 'Country'],
						range: { min: '0%', max: '110%' }
					},
					x: { attach: ['Value 3 (+)'], range: { min: '0%', max: '110%' } },
					color: { attach: ['Country'] },
					size: { attach: ['Country'] },
					label: { attach: ['Country'] }
				},
				title: 'Size Discrete Anim.',
				legend: 'size',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) =>
					record.Country !== 'Germany' &&
					record.Country !== 'Italy' &&
					record.Country !== 'Hungary' &&
					record.Country !== 'Netherlands' &&
					record.Country !== 'Finland' &&
					record.Country !== 'Lithuania' &&
					record.Country !== 'Latvia' &&
					record.Country !== 'Croatia' &&
					record.Country !== 'Ireland' &&
					record.Country !== 'Denmark' &&
					record.Country !== 'Hungary' &&
					record.Country !== 'United Kingdom'
			},

			config: {
				title: 'Size Discrete Anim. - Filtered'
			}
		})
]

export default testSteps
