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
					label: { attach: ['Country'] }
				},
				title: 'Color Discrete Anim.',
				legend: 'color',
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
					record.Country !== 'Estonia' &&
					record.Country !== 'Belgium' &&
					record.Country !== 'Sweden' &&
					record.Country !== 'Poland' &&
					record.Country !== 'Malta' &&
					record.Country !== 'Romania' &&
					record.Country !== 'Bulgaria' &&
					record.Country !== 'Slovakia' &&
					record.Country !== 'Ireland' &&
					record.Country !== 'Lithuania' &&
					record.Country !== 'United Kingdom'
			},
			config: {
				title: 'Color Discrete Anim. - Filtered'
			}
		})
]

export default testSteps
