import { data_6 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate(
			{
				data: data_6,
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: 'Value 2 (+)' },
						color: { set: 'Country' }
					},
					title: '1 Line',
					geometry: 'line'
				},
				style: {
					title: {
						fontSize: '2em'
					}
				}
			},
			{
				delay: 0
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: ['Value 2 (+)', 'Country'] },
					color: { set: 'Country' }
				},
				title: '2 Stacked Area / Sum(?), Stack(?)',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: 'Value 2 (+)' },
						color: { set: 'Country' }
					},
					title: '3 Line / Comparison(?), Components(?)',
					geometry: 'line'
				}
			},
			{
				delay: 1
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: ['Value 2 (+)', 'Country'], split: true },
						color: { set: 'Country' }
					},
					title: '4 Trellis Area / Trellis(?), Components(?), Part-to-whole(?) (sort???)',
					geometry: 'area'
				}
			},
			{
				delay: 1
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: 'Value 2 (+)', split: false },
						color: { set: 'Country' }
					},
					title: '5 Line / Comparison(?)',
					geometry: 'line'
				}
			},
			{
				delay: 1
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: ['Value 2 (+)', 'Country'], align: 'stretch' },
					color: { set: 'Country' }
				},
				title: '6 Stacked Area / Ratio(?)',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: 'Value 2 (+)', align: 'none' },
						color: { set: 'Country' }
					},
					title: '7 Line / Comparison(?), Components(?)',
					geometry: 'line'
				}
			},
			{
				delay: 1
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: 'Value 2 (+)' },
						size: { set: 'Country' },
						color: { set: null }
					},
					title: '8 Line / Aggregate(?), - Discrete(?)',
					geometry: 'line'
				}
			},
			{
				delay: 1
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: 'Value 2 (+)' },
						color: { set: 'Country' },
						size: { set: null }
					},
					title: '9 Line / Drill down(?), + Discrete(?)',
					geometry: 'line'
				}
			},
			{
				delay: 1
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Year', 'Value 2 (+)'] },
					y: { set: 'Country' },
					color: { set: null }
				},
				title: '10 Bar / Sum(?)',
				geometry: 'rectangle',
				legend: null
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Value 2 (+)'] },
						y: { set: 'Country' },
						color: { set: null },
						label: { set: 'Value 2 (+)' }
					},
					title: '11 Bar / Sum(?)',
					geometry: 'rectangle',
					legend: null
				}
			},
			{
				delay: 0
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Year', 'Value 2 (+)'] },
						y: { set: 'Country' },
						color: { set: null },
						label: { set: null }
					},
					title: '12 Bar / Sum(?)',
					geometry: 'rectangle',
					legend: null
				}
			},
			{
				delay: 1
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Value 2 (+)' },
					color: { set: 'Country' },
					size: { set: null }
				},
				title: '13 Line / Drill down timeseries(?), + Discrete(?)',
				geometry: 'line',
				legend: 'color'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: ['Country', 'Value 2 (+)'] },
					color: { set: 'Country' }
				},
				title: '14 Stacked Column / Sum(?), Stack(?)',
				geometry: 'rectangle',
				legend: 'color'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Value 2 (+)' },
					color: { set: 'Country' }
				},
				title: '15 Line / Comparison(?), + Components(?)',
				geometry: 'line'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: ['Country', 'Value 2 (+)'], split: true },
					color: { set: 'Country' }
				},
				title: '16 Line / Comparison(?), + Components(?)',
				geometry: 'line'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Country', split: false },
					color: { set: null },
					lightness: { set: 'Value 2 (+)' }
				},
				title: '17 Heatmap / (?), (?), (sort???)',
				geometry: 'rectangle',
				legend: null
			}
		})
]

export default testSteps
