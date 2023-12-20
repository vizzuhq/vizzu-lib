import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	async (chart) => {
		await import('https://d3js.org/d3.v7.min.js')

		return chart.animate({
			data,
			config: {
				x: 'Joy factors',
				y: 'Value 2 (+)',
				color: 'Joy factors',
				title: 'D3 Color Palettes'
			}
		})
	},
	(chart) =>
		chart.animate({
			config: { title: 'Category10' },
			style: { 'plot.marker.colorPalette': d3.schemeCategory10.join(' ') }
		}),
	(chart) =>
		chart.animate({
			config: { title: 'Accent' },
			style: { 'plot.marker.colorPalette': d3.schemeAccent.join(' ') }
		}),
	(chart) =>
		chart.animate({
			config: { title: 'Dark2' },
			style: { 'plot.marker.colorPalette': d3.schemeDark2.join(' ') }
		}),
	(chart) =>
		chart.animate({
			config: { title: 'Paired' },
			style: { 'plot.marker.colorPalette': d3.schemePaired.join(' ') }
		}),
	(chart) =>
		chart.animate({
			config: { title: 'Pastel1' },
			style: { 'plot.marker.colorPalette': d3.schemePastel1.join(' ') }
		}),
	(chart) =>
		chart.animate({
			config: { title: 'Pastel2' },
			style: { 'plot.marker.colorPalette': d3.schemePastel2.join(' ') }
		}),
	(chart) =>
		chart.animate({
			config: { title: 'Set1' },
			style: { 'plot.marker.colorPalette': d3.schemeSet1.join(' ') }
		}),
	(chart) =>
		chart.animate({
			config: { title: 'Set2' },
			style: { 'plot.marker.colorPalette': d3.schemeSet2.join(' ') }
		}),
	(chart) =>
		chart.animate({
			config: { title: 'Set3' },
			style: { 'plot.marker.colorPalette': d3.schemeSet3.join(' ') }
		}),
	(chart) =>
		chart.animate({
			config: { title: 'Tableau10' },
			style: { 'plot.marker.colorPalette': d3.schemeTableau10.join(' ') }
		})
]

export default testSteps
