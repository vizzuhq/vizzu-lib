import { data_6 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: {
				channels: {
					x: { set: 'Year' },
					y: {
						set: ['Country', 'Value 2 (+)'],
						range: { min: '0%', max: '100%' },
						split: true
					},
					color: { set: 'Country' }
				},
				title: '1 Splitted Column',
				geometry: 'rectangle',
				orientation: 'horizontal'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Year', 'Country'] },
					y: { set: ['Value 2 (+)'], range: { min: '0%', max: '100%' }, split: false },
					color: { set: 'Country' }
				},
				title: '2 Groupped Column / Comparison(?), Group(?)',
				geometry: 'rectangle',
				orientation: 'horizontal'
			},
			style: {
				title: {
					fontSize: '2em'
				}
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
							range: { min: '0%', max: '100%' },
							split: true
						},
						color: { set: 'Country' }
					},
					title: '3 Splitted Column / Components(?), Part-to-whole(?), Split(?)',
					geometry: 'rectangle',
					orientation: 'horizontal'
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
					x: { set: ['Country', 'Year'] },
					y: { set: ['Value 2 (+)'], range: { min: '0%', max: '100%' }, split: false },
					color: { set: 'Country' }
				},
				title: '4 Groupped Column / Comparison(?), Group(?)',
				geometry: 'rectangle',
				orientation: 'horizontal'
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
							range: { min: '0%', max: '100%' },
							split: true
						},
						color: { set: 'Country' }
					},
					title: '5 Splitted Column / Components(?), Part-to-whole(?), Split(?)',
					geometry: 'rectangle',
					orientation: 'horizontal'
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
					y: {
						set: ['Country', 'Value 2 (+)'],
						range: { min: '0%', max: '100%' },
						split: false
					},
					color: { set: 'Country' }
				},
				title: '6 Stacked Column / Sum(?), Stack(?)',
				geometry: 'rectangle',
				orientation: 'horizontal'
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
							range: { min: '0%', max: '100%' },
							split: true
						},
						color: { set: 'Country' }
					},
					title: '7 Splitted Column / Components(?), Part-to-whole(?), Split(?)',
					geometry: 'rectangle',
					orientation: 'horizontal'
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
					y: { set: ['Country', 'Value 2 (+)'], align: 'stretch', split: false }
				},
				title: '8 Stacked Percentage Column / Ratio%(?)',
				geometry: 'rectangle',
				orientation: 'horizontal'
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
							range: { min: '0%', max: '100%' },
							align: 'none',
							split: true
						},
						color: { set: 'Country' }
					},
					title: '9 Splitted Column / Components(?), Part-to-whole(?), Split(?)',
					geometry: 'rectangle',
					orientation: 'horizontal'
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
					x: { set: 'Value 5 (+/-)' },
					y: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' }, split: false },
					color: { set: 'Country' },
					noop: { set: 'Year' }
				},
				title: '10 Scatter plot / + Continuous(?)',
				geometry: 'circle',
				orientation: 'horizontal'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: {
						set: ['Country', 'Value 2 (+)'],
						range: { min: '0%', max: '100%' },
						split: true
					},
					color: { set: 'Country' },
					noop: { set: null }
				},
				title: '11 Splitted Column / - Continuous(?), Components(?), Part-to-whole(?), Split(?)',
				geometry: 'rectangle',
				orientation: 'horizontal'
			},
			style: {
				title: {
					fontSize: '1.8em'
				}
			}
		}),

	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
