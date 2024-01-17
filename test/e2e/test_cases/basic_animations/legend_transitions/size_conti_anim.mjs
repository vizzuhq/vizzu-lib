import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
					x: { attach: ['Value 6 (+/-)'], range: { min: '0%', max: '110%' } },
					color: { attach: ['Country'] },
					size: { attach: ['Value 4 (+/-)'] },
					label: { attach: ['Value 4 (+/-)'] }
				},
				title: 'Size Continuous Anim.',
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
				title: 'Size Continuous Anim. - Filtered'
			}
		})
]

export default testSteps
