import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Year'] },
					y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
					label: { attach: ['Value 5 (+/-)'] },
					size: { attach: ['Value 2 (+)'] }
				},
				title: 'Histogram with (-) Nums'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Circle plot with (-) Nums',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Histogram with (-) Nums',
				geometry: 'rectangle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Area with (-) Nums',
				geometry: 'area'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Histogram with (-) Nums',
				geometry: 'rectangle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Line with (-) Nums',
				geometry: 'line'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Circle with (-) Nums',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Area with (-) Nums',
				geometry: 'area'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Circle with (-) Nums',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Line with (-) Nums',
				geometry: 'line'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Area with (-) Nums',
				geometry: 'area'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Circle with (-) Nums',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Area with (-) Nums',
				geometry: 'area'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Line with (-) Nums',
				geometry: 'line'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Circle with (-) Nums',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Line with (-) Nums',
				geometry: 'line'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Area with (-) Nums',
				geometry: 'area'
			}
		})
]

export default testSteps
