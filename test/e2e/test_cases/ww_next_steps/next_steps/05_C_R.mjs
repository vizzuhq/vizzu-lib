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
						range: { min: '0%', max: '100%' }
					},
					color: { set: 'Country' }
				},
				title: 'Splitted Column',
				geometry: 'rectangle',
				orientation: 'horizontal',
				split: true
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Year', 'Country'] },
					y: { set: ['Value 2 (+)'], range: { min: '0%', max: '100%' } },
					color: { set: 'Country' }
				},
				title: 'Groupped Column / Comparison(?), Group(?)',
				geometry: 'rectangle',
				orientation: 'horizontal',
				split: false
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
							range: { min: '0%', max: '100%' }
						},
						color: { set: 'Country' }
					},
					title: 'Splitted Column / Components(?), Part-to-whole(?), Split(?)',
					geometry: 'rectangle',
					orientation: 'horizontal',
					split: true
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
					y: { set: ['Value 2 (+)'], range: { min: '0%', max: '100%' } },
					color: { set: 'Country' }
				},
				title: 'Groupped Column / Comparison(?), Group(?)',
				geometry: 'rectangle',
				orientation: 'horizontal',
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
							set: ['Country', 'Value 2 (+)'],
							range: { min: '0%', max: '100%' }
						},
						color: { set: 'Country' }
					},
					title: 'Splitted Column / Components(?), Part-to-whole(?), Split(?)',
					geometry: 'rectangle',
					orientation: 'horizontal',
					split: true
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
						range: { min: '0%', max: '100%' }
					},
					color: { set: 'Country' }
				},
				title: 'Stacked Column / Sum(?), Stack(?)',
				geometry: 'rectangle',
				orientation: 'horizontal',
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
							set: ['Country', 'Value 2 (+)'],
							range: { min: '0%', max: '100%' }
						},
						color: { set: 'Country' }
					},
					title: 'Splitted Column / Components(?), Part-to-whole(?), Split(?)',
					geometry: 'rectangle',
					orientation: 'horizontal',
					split: true
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
					y: { set: ['Country', 'Value 2 (+)'] }
				},
				title: 'Stacked Percentage Column / Ratio%(?)',
				geometry: 'rectangle',
				orientation: 'horizontal',
				align: 'stretch',
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
							set: ['Country', 'Value 2 (+)'],
							range: { min: '0%', max: '100%' }
						},
						color: { set: 'Country' }
					},
					title: 'Splitted Column / Components(?), Part-to-whole(?), Split(?)',
					geometry: 'rectangle',
					orientation: 'horizontal',
					align: 'min',
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
						x: { set: 'Value 5 (+/-)' },
						y: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' } },
						color: { set: 'Country' },
						noop: { set: 'Year' }
					},
					title: 'Scatter plot / + Continuous(?)',
					geometry: 'circle',
					orientation: 'horizontal',
					align: 'min',
					split: false
				}
			},
			{
				style: {
					delay: 0.5,
					duration: 1.5
				},
				geometry: {
					delay: 0,
					duration: 1.3
				},
				x: {
					delay: 1,
					duration: 1
					//                easing: 'ease-out'
				},
				y: {
					delay: 1,
					duration: 1
					//             easing: 'cubic-bezier(.39,0,.35,.99)'
				}
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
							range: { min: '0%', max: '100%' }
						},
						color: { set: 'Country' },
						noop: { set: null }
					},
					title: 'Splitted Column / - Continuous(?), Components(?), Part-to-whole(?), Split(?)',
					geometry: 'rectangle',
					orientation: 'horizontal',
					align: 'min',
					split: true
				},
				style: {
					title: {
						fontSize: '1.8em'
					}
				}
			},
			{
				style: {
					delay: 1.3,
					duration: 1.5
				},
				geometry: {
					delay: 2,
					duration: 1.3
				},
				x: {
					delay: 1,
					duration: 1
					//                easing: 'ease-out'
				},
				y: {
					delay: 1,
					duration: 1
					//             easing: 'cubic-bezier(.39,0,.35,.99)'
				}
			}
		),

	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
