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
				title: 'Area',
				geometry: 'area'
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
						y: { set: ['Value 2 (+)', 'Country'] },
						color: { set: 'Country' }
					},
					title: 'Stacked Area / Drill down(?), + Discrete(?)',
					geometry: 'area'
				}
			},
			{
				coordSystem: {
					delay: 0,
					duration: 1
				},
				geometry: {
					delay: 0,
					duration: 0.75
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

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: 'Value 2 (+)' },
						color: { set: 'Country' }
					},
					title: 'Line Chart / Comparison(?), Components(?), ',
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
					title: 'Trellis Area / Trellis(?), Components(?), Part-to-whole(?) (sort???)',
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
						y: { set: ['Value 2 (+)', 'Country'], split: false },
						color: { set: 'Country' }
					},
					title: 'Stacked Area / Trellis off(?), Merge(?)',
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
						y: { set: ['Value 2 (+)', 'Country'], align: 'stretch' },
						color: { set: 'Country' }
					},
					title: 'Stacked Area / Ratio(?)',
					geometry: 'area'
				}
			},
			{
				coordSystem: {
					delay: 0,
					duration: 1
				},
				geometry: {
					delay: 0,
					duration: 0.75
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

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: ['Value 2 (+)', 'Country'], align: 'none' },
						color: { set: 'Country' }
					},
					title: 'Stacked Area / Ratio off(?)',
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
						y: {
							set: ['Value 2 (+)', 'Country'],
							range: { min: '0%', max: '100%' },
							split: true
						},
						color: { set: 'Country' }
					},
					title: 'Bar / Sum(?)',
					geometry: 'area'
				}
			},
			{
				delay: 1,
				duration: 1
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Year', 'Value 2 (+)'] },
						y: { set: ['Country'], split: false },
						color: { set: null }
					},
					title: 'Bar / Sum(?)',
					geometry: 'rectangle'
				}
			},
			{
				delay: 0,
				duration: 1,
				geometry: {
					delay: 0,
					duration: 0.5
				},
				coordSystem: {
					delay: 0,
					duration: 0.5
				},
				x: {
					delay: 0,
					duration: 1
				},
				y: {
					delay: 0,
					duration: 0.5
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
						label: { set: 'Value 2 (+)' }
					},
					title: 'Bar / Sum(?)',
					geometry: 'rectangle'
				}
			},
			{
				delay: 0,
				duration: 0.3
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Year', 'Value 2 (+)'] },
						y: { set: ['Country'] },
						color: { set: null },
						label: { set: null }
					},
					title: 'Bar / Sum(?)',
					geometry: 'rectangle'
				}
			},
			{
				delay: 1,
				duration: 1,
				geometry: {
					delay: 0,
					duration: 1
				},
				coordSystem: {
					delay: 0,
					duration: 0.5
				},
				x: {
					delay: 0,
					duration: 1
				},
				y: {
					delay: 0,
					duration: 0.5
					//                easing: 'cubic-bezier(.39,0,.35,.99)'
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
							set: ['Value 2 (+)', 'Country'],
							range: { min: '0%', max: '110%' },
							split: true
						},
						color: { set: 'Country' }
					},
					title: 'Area / Time distribution(?)',
					geometry: 'area'
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
						y: { set: ['Value 2 (+)', 'Country'], split: false },
						color: { set: 'Country' }
					},
					title: 'Stacked Area / Time distribution(?)',
					geometry: 'area'
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
					title: 'Area / Total timeseries(?)',
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
						y: { set: ['Value 2 (+)', 'Country'] },
						color: { set: 'Country' }
					},
					title: 'Stacked Area / Time distribution(?)',
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
						y: { set: ['Value 2 (+)'] },
						color: { set: 'Country' }
					},
					title: '16 Lollipop / Distribution(?)',
					geometry: 'circle'
				}
			},
			{
				delay: 1,
				duration: 2,
				geometry: {
					delay: 0,
					duration: 0.75
				},
				coordSystem: {
					delay: 0,
					duration: 0.5
				},
				x: {
					delay: 0.5,
					duration: 1
				},
				y: {
					delay: 0.5,
					duration: 1
					//                easing: 'cubic-bezier(.39,0,.35,.99)'
				}
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
					title: 'Stacked Area / Sum(?), Sum timeseries(?)',
					geometry: 'area'
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
