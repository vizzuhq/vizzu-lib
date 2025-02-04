import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate(
			{
				data: data_14,
				config: {
					channels: {
						x: { set: 'Year' },
						y: {
							set: ['Country', 'Value 2 (+)'],
							range: { min: '0%', max: '110%' }
						},
						color: { set: 'Country' }
					},
					title: '1 Stacked Column',
					geometry: 'rectangle',
					legend: 'color'
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
						x: { set: null },
						y: {
							set: ['Country', 'Value 2 (+)'],
							range: { min: '0%', max: '110%' }
						},
						color: { set: 'Country' },
						label: { set: ['Value 2 (+)'] }
					},
					title: '2 Stacked Column / Aggregate(?), Sum(?), - Discrete(?)',
					geometry: 'rectangle'
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
						y: {
							set: ['Country', 'Value 2 (+)'],
							range: { min: '0%', max: '110%' }
						},
						color: { set: 'Country' },
						label: null
					},
					title: '3 Stacked Column',
					geometry: 'rectangle',
					legend: 'color'
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
						x: { set: ['Value 2 (+)'] },
						y: { set: ['Country'] },
						color: { set: null },
						label: { set: ['Value 2 (+)'] }
					},
					title: '4 Bar / Aggregate(?), Sum(?)',
					geometry: 'rectangle',
					legend: null
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
						y: {
							set: ['Country', 'Value 2 (+)'],
							range: { min: '0%', max: '110%' }
						},
						color: { set: 'Country' },
						label: null
					},
					title: '5 Stacked Column / Drill down(?), + Discrete(?)',
					geometry: 'rectangle',
					legend: 'color'
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
						x: { set: null },
						y: { set: ['Value 2 (+)', 'Country'] },
						color: { set: ['Country'] }
					},
					title: '6 Column / Total(?), Aggregate',
					geometry: 'rectangle',
					legend: 'color'
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
						x: { set: null },
						y: { set: ['Value 2 (+)'] },
						color: { set: null },
						label: { set: ['Value 2 (+)'] }
					},
					title: '7 Column / Total(?), Aggregate',
					geometry: 'rectangle',
					legend: null
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
						y: {
							set: ['Country', 'Value 2 (+)'],
							range: { min: '0%', max: '110%' }
						},
						color: { set: 'Country' },
						label: null
					},
					title: '9 Stacked Column / Drill down(?), + Discrete(?)',
					geometry: 'rectangle',
					legend: 'color'
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
						x: { set: ['Year', 'Country'] },
						y: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' } },
						color: { set: 'Country' }
					},
					title: '10 Groupped Column / Comparison(?), Group(?)',
					geometry: 'rectangle',
					legend: null
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
						y: {
							set: ['Country', 'Value 2 (+)'],
							range: { min: '0%', max: '110%' }
						},
						color: { set: 'Country' }
					},
					title: '11 Stacked Column / Sum(?), Stack(?)',
					geometry: 'rectangle',
					legend: 'color'
				}
			},
			{
				delay: 0,
				duration: 1
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: {
						set: ['Country', 'Value 2 (+)'],
						range: { min: '0%', max: '110%' },
						split: true
					},
					color: { set: 'Country' }
				},
				title: '12 Splitted Column / Components(?)',
				geometry: 'rectangle',
				legend: 'color'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: {
							set: ['Country', 'Value 2 (+)'],
							range: { min: '0%', max: '110%' },
							split: false
						},
						color: { set: 'Country' }
					},
					title: '13 Stacked Column / Sum(?), Stack(?)',
					geometry: 'rectangle',
					legend: 'color'
				}
			},
			{
				delay: 0,
				duration: 1
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: {
						set: ['Country', 'Value 2 (+)'],
						range: { min: '0%', max: '100%' },
						align: 'stretch'
					},
					color: { set: 'Country' }
				},
				title: '14 Stacked Percentage Column / Ratio%(?)'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: {
							set: ['Country', 'Value 2 (+)'],
							range: { min: '0%', max: '110%' },
							align: 'none'
						},
						color: { set: 'Country' }
					},
					title: '15 Stacked Column / Sum(?), Stack(?)',
					geometry: 'rectangle',
					legend: 'color'
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
						x: { set: 'Value 5 (+/-)' },
						y: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' } },
						color: { set: 'Country' },
						noop: { set: 'Year' }
					},
					title: '16 Scatter plot / + Continuous(?)',
					geometry: 'circle',
					legend: 'color'
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
						y: {
							set: ['Country', 'Value 2 (+)'],
							range: { min: '0%', max: '110%' }
						},
						color: { set: 'Country' },
						noop: { set: null }
					},
					title: '17 Stacked Column / - Continuous(?)',
					geometry: 'rectangle',
					legend: 'color'
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
						y: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' } },
						color: { set: 'Country' }
					},
					title: '18 Lollipop / Distribute(?)',
					geometry: 'circle'
				}
			},
			{
				delay: 0,
				duration: 1
			}
		),

	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
