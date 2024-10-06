import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
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
				title: 'Stacked Column',
				geometry: 'rectangle',
				split: false,
				legend: 'color'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: null },
						y: {
							set: ['Country', 'Year', 'Value 2 (+)'],
							range: { min: '0%', max: '110%' }
						},
						color: { set: 'Country' }
					},
					title: 'Stacked Column / Aggregate(?), Sum(?), - Discrete(?)',
					geometry: 'rectangle',
					split: false,
					legend: 'color'
				}
			},
			{
				duration: 2
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
					title: 'Stacked Column / Aggregate(?), Sum(?), - Discrete(?)',
					geometry: 'rectangle',
					split: false
				}
			},
			{
				delay: 0,
				duration: 0.5
			}
		),
	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: null },
						y: {
							set: ['Country', 'Year', 'Value 2 (+)'],
							range: { min: '0%', max: '110%' }
						},
						color: { set: 'Country' },
						label: { set: null }
					},
					title: 'Stacked Column / Drill down(?), + Discrete(?)',
					geometry: 'rectangle',
					split: false,
					legend: 'color'
				}
			},
			{
				delay: 1,
				duration: 2
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: {
						set: ['Country', 'Value 2 (+)'],
						range: { min: '0%', max: '110%' }
					},
					color: { set: 'Country' }
				},
				title: 'Stacked Column',
				geometry: 'rectangle',
				split: false,
				legend: 'color'
			}
		}),
	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Value 2 (+)', 'Year'] },
						y: { set: ['Country'], range: { min: '0%', max: '100%' } },
						color: { set: null }
					},
					title: 'Bar / Aggregate(?), Sum(?)',
					geometry: 'rectangle',
					split: false,
					legend: null
				}
			},
			{
				easing: 'cubic-bezier(0.65,0,0.65,1)',
				geometry: {
					delay: 0.5,
					duration: 0.5
					//                easing: 'linear'
				},
				x: {
					delay: 0.5,
					duration: 0.75
					//                easing: 'ease-out'
				},
				y: {
					delay: 0,
					duration: 0.75
					//                easing: 'cubic-bezier(.39,0,.35,.99)'
				}
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
					title: 'Bar / Aggregate(?), Sum(?)',
					geometry: 'rectangle',
					split: false,
					legend: null
				}
			},
			{
				delay: 0,
				duration: 0.4
			}
		),
	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Value 2 (+)', 'Year'] },
						y: { set: ['Country'] },
						color: { set: null },
						label: { set: null }
					},
					title: 'Bar / Aggregate(?), Sum(?)',
					geometry: 'rectangle',
					split: false,
					legend: null
				}
			},
			{
				delay: 2,
				duration: 0.4
			}
		),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: {
						set: ['Country', 'Value 2 (+)'],
						range: { min: '0%', max: '110%' }
					},
					color: { set: 'Country' }
				},
				title: 'Stacked Column / Drill down(?), + Discrete(?)',
				geometry: 'rectangle',
				split: false,
				legend: 'color'
			}
		}),
	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Year', 'Value 2 (+)'] },
						y: { set: ['Country'] },
						color: { set: ['Country'] },
						label: { set: null }
					},
					title: '1 Column / Total(?), Aggregate',
					geometry: 'rectangle',
					split: false,
					legend: null
				}
			},
			{
				delay: 0,
				duration: 1,
				coordSystem: {
					delay: 0,
					duration: 1
				},
				x: {
					delay: 0.75,
					duration: 1,
					easing: 'ease-out'
				},
				y: {
					delay: 0,
					duration: 1,
					easing: 'ease-out'
				}
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: null },
						y: { set: ['Value 2 (+)', 'Year', 'Country'] },
						color: { set: null }
					},
					title: '2 Column / Total(?), Aggregate',
					geometry: 'rectangle',
					split: false,
					legend: null
				}
			},
			{
				delay: 0,
				duration: 2,
				x: {
					delay: 0,
					duration: 1,
					easing: 'ease-out'
				},
				y: {
					delay: 1,
					duration: 1,
					easing: 'ease-in'
				}
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
					title: '3 Column / Total(?), Aggregate',
					geometry: 'rectangle',
					split: false,
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
						x: { set: null },
						y: { set: ['Value 2 (+)', 'Country', 'Year'] },
						color: { set: null },
						label: { set: null }
					},
					title: '1 Column / Sum(?)',
					geometry: 'rectangle',
					split: false,
					legend: null
				}
			},
			{
				delay: 2,
				duration: 1
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: null },
						y: { set: ['Value 2 (+)', 'Country', 'Year'] },
						color: { set: 'Country' },
						label: { set: null }
					},
					title: '2 Stacked Column / Drill down(?), + Discrete(?)',
					geometry: 'rectangle',
					split: false,
					legend: 'color'
				}
			},
			{
				delay: 0,
				duration: 2
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Year', 'Value 2 (+)'] },
						y: { set: ['Country'] },
						color: { set: 'Country' },
						label: { set: null }
					},
					title: '3 Stacked Column / Drill down(?), + Discrete(?)',
					geometry: 'rectangle',
					split: false,
					legend: null
				}
			},
			{
				delay: 0,
				duration: 2
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
					title: '4 Stacked Column / Drill down(?), + Discrete(?)',
					geometry: 'rectangle',
					split: false,
					legend: 'color'
				}
			},
			{
				delay: 0,
				duration: 2
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
					title: 'Groupped Column / Comparison(?), Group(?)',
					geometry: 'rectangle',
					split: false,
					legend: null
				}
			},
			{
				delay: 2,
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
					title: 'Stacked Column / Sum(?), Stack(?)',
					geometry: 'rectangle',
					split: false,
					legend: 'color'
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
						range: { min: '0%', max: '110%' }
					},
					color: { set: 'Country' }
				},
				title: 'Splitted Column / Components(?)',
				geometry: 'rectangle',
				split: true,
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
							range: { min: '0%', max: '110%' }
						},
						color: { set: 'Country' }
					},
					title: 'Stacked Column / Sum(?), Stack(?)',
					geometry: 'rectangle',
					split: false,
					legend: 'color'
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
				title: 'Stacked Percentage Column / Ratio%(?)',
				align: 'stretch'
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
							range: { min: '0%', max: '110%' }
						},
						color: { set: 'Country' }
					},
					title: 'Stacked Column / Sum(?), Stack(?)',
					geometry: 'rectangle',
					split: false,
					legend: 'color',
					align: 'none'
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
					split: false,
					legend: 'color',
					align: 'none'
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
					delay: 1.25,
					duration: 1
					//                easing: 'ease-out'
				},
				y: {
					delay: 0.75,
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
							range: { min: '0%', max: '110%' }
						},
						color: { set: 'Country' },
						noop: { set: null }
					},
					title: 'Stacked Column / - Continuous(?)',
					geometry: 'rectangle',
					split: false,
					legend: 'color',
					align: 'none'
				}
			},
			{
				delay: 1,
				style: {
					delay: 1.3,
					duration: 1.5
				},
				geometry: {
					delay: 1.5,
					duration: 1.3
				},
				x: {
					delay: 0,
					duration: 1
					//                easing: 'ease-out'
				},
				y: {
					delay: 0.75,
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
						y: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' } },
						color: { set: 'Country' }
					},
					title: 'Lollipop / Distribute(?)',
					geometry: 'circle',
					split: false
				}
			},
			{
				coordSystem: {
					delay: 0,
					duration: 1
				},
				geometry: {
					delay: 0,
					duration: 0.5
					//                easing: 'linear'
				},
				x: {
					delay: 0.5,
					duration: 0.5
					//                easing: 'ease-out'
				},
				y: {
					delay: 0,
					duration: 0.75
					//                easing: 'cubic-bezier(.39,0,.35,.99)'
				}
			}
		),

	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
