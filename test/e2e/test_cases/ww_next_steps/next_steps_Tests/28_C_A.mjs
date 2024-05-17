import { data_6 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Value 2 (+)' },
					color: { set: null }
				},
				title: '1 Area',
				geometry: 'area'
			},
			style: {
				title: {
					fontSize: '2em'
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: ['Value 2 (+)', 'Country'] },
					color: { set: 'Country' }
				},
				title: '2 Stacked Area / Drill down(?), + Discrete(?)',
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
					title: '3 Line Chart / Comparison(?), Components(?), ',
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
						y: { set: ['Value 2 (+)', 'Country'] },
						color: { set: 'Country' }
					},
					title: '4 Trellis Area / Trellis(?), Components(?), Part-to-whole(?) (sort???)',
					geometry: 'area',
					split: true
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
						y: { set: ['Value 2 (+)', 'Country'] },
						color: { set: 'Country' }
					},
					title: '5 Stacked Area / Trellis off(?), Merge(?)',
					geometry: 'area',
					split: false
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
					y: { set: ['Value 2 (+)', 'Country'] },
					color: { set: 'Country' }
				},
				title: '6 Stacked Area / Ratio(?)',
				geometry: 'area',
				align: 'stretch'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: ['Value 2 (+)', 'Country'] },
						color: { set: 'Country' }
					},
					title: '7 Stacked Area / Ratio off(?)',
					geometry: 'area',
					align: 'min',
					split: false
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
						y: {
							set: ['Value 2 (+)', 'Country'],
							range: { min: '0%', max: '100%' }
						},
						color: { set: 'Country' }
					},
					title: '8 Bar / Sum(?)',
					geometry: 'area',
					align: 'min',
					split: true
				}
			},
			{
				delay: 1,
				duration: 1
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Year', 'Value 2 (+)'] },
					y: { set: ['Country'] },
					color: { set: null }
				},
				title: '9 Bar / Sum(?)',
				geometry: 'rectangle',
				align: 'min',
				split: false
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Value 2 (+)'] },
						y: { set: ['Country'] },
						color: { set: null },
						label: { set: 'Value 2 (+)' }
					},
					title: '10 Bar / Sum(?)',
					geometry: 'rectangle',
					align: 'min',
					split: false
				}
			},
			{
				delay: 0,
				duration: 0.3
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Year', 'Value 2 (+)'] },
					y: { set: ['Country'] },
					color: { set: null },
					label: { set: null }
				},
				title: '11 Bar / Sum(?)',
				geometry: 'rectangle',
				align: 'min',
				split: false
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: {
							set: ['Value 2 (+)', 'Country'],
							range: { min: '0%', max: '110%' }
						},
						color: { set: 'Country' }
					},
					title: '12 Area / Time distribution(?)',
					geometry: 'area',
					align: 'min',
					split: true
				}
			},
			{
				delay: 0,
				duration: 1
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: ['Value 2 (+)', 'Country'] },
						color: { set: 'Country' }
					},
					title: '13 Stacked Area / Time distribution(?)',
					geometry: 'area',
					align: 'min',
					split: false
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
						x: { set: 'Year' },
						y: { set: ['Value 2 (+)'] },
						color: { set: null }
					},
					title: '14 Area / Total timeseries(?)',
					geometry: 'area',
					align: 'min',
					split: false
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
						y: { set: ['Value 2 (+)', 'Country'] },
						color: { set: 'Country' }
					},
					title: '15 Stacked Area / Time distribution(?)',
					geometry: 'area',
					align: 'min',
					split: false
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
					y: { set: ['Value 2 (+)'] },
					color: { set: 'Country' }
				},
				title: '16 Lollipop / Distribution(?)',
				geometry: 'circle',
				align: 'min',
				split: false
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: ['Value 2 (+)', 'Country'] },
						color: { set: 'Country' }
					},
					title: '17 Stacked Area / Sum(?), Sum timeseries(?)',
					geometry: 'area',
					align: 'min',
					split: false
				}
			},
			{
				delay: 1
			}
		),

	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
