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
					title: 'Line',
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
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: ['Value 2 (+)', 'Country'] },
						color: { set: 'Country' }
					},
					title: 'Stacked Area / Sum(?), Stack(?)',
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
					title: 'Line / Comparison(?), Components(?)',
					geometry: 'line',
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
					title: 'Trellis Area / Trellis(?), Components(?), Part-to-whole(?) (sort???)',
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
						y: { set: 'Value 2 (+)' },
						color: { set: 'Country' }
					},
					title: 'Line / Comparison(?)',
					geometry: 'line',
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
					title: 'Stacked Area / Ratio(?)',
					geometry: 'area',
					align: 'stretch'
				}
			},
			{
				delay: 1,
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
					title: 'Line / Comparison(?), Components(?)',
					geometry: 'line',
					align: 'min'
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
						y: { set: ['Country', 'Value 2 (+)'] },
						size: { set: null },
						color: { set: 'Country' }
					},
					title: 'Line / Aggregate(?), - Discrete(?)',
					geometry: 'area',
					align: 'min',
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
					size: { set: null },
					color: { set: null }
				},
				title: '2222 Line / Aggregate(?), - Discrete(?)',
				geometry: 'line',
				align: 'min'
			}
		}),

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
					title: 'Line / Drill down(?), + Discrete(?)',
					geometry: 'line',
					align: 'min'
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
						x: { set: ['Year', 'Value 2 (+)'] },
						y: { set: 'Country' },
						color: { set: null }
					},
					title: 'Bar / Sum(?)',
					geometry: 'rectangle',
					align: 'min',
					legend: null
				}
			},
			{
				geometry: {
					delay: 0.5,
					duration: 1
					//                easing: 'linear'
				},
				x: {
					delay: 0.5,
					duration: 1
					//               easing: 'ease-in'
				},
				y: {
					delay: 0,
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
						x: { set: ['Value 2 (+)'] },
						y: { set: 'Country' },
						color: { set: null },
						label: { set: 'Value 2 (+)' }
					},
					title: 'Bar / Sum(?)',
					geometry: 'rectangle',
					align: 'min',
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
					title: 'Bar / Sum(?)',
					geometry: 'rectangle',
					align: 'min',
					legend: null
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
					title: 'Line / Drill down timeseries(?), + Discrete(?)',
					geometry: 'line',
					align: 'min',
					legend: 'color'
				}
			},
			{
				delay: 0,
				geometry: {
					delay: 0,
					duration: 1
					//                easing: 'linear'
				},
				x: {
					delay: 0,
					duration: 1
					//               easing: 'ease-in'
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
						y: { set: ['Country', 'Value 2 (+)'] },
						color: { set: 'Country' }
					},
					title: 'Stacked Column / Sum(?), Stack(?)',
					geometry: 'rectangle',
					align: 'min',
					legend: 'color'
				}
			},
			{
				delay: 1,
				geometry: {
					delay: 0,
					duration: 1
					//                easing: 'linear'
				},
				x: {
					delay: 0,
					duration: 0.5
					//               easing: 'ease-in'
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
						y: { set: 'Value 2 (+)' },
						color: { set: 'Country' }
					},
					title: 'Line / Comparison(?), + Components(?)',
					geometry: 'line',
					align: 'min'
				}
			},
			{
				delay: 0,
				geometry: {
					delay: 0,
					duration: 1
					//                easing: 'linear'
				},
				x: {
					delay: 0,
					duration: 1
					//               easing: 'ease-in'
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
						y: { set: ['Country', 'Value 2 (+)'] },
						color: { set: 'Country' }
					},
					title: 'Line / Comparison(?), + Components(?)',
					geometry: 'line',
					split: true
				}
			},
			{
				delay: 0,
				geometry: {
					delay: 0,
					duration: 1
					//                easing: 'linear'
				},
				x: {
					delay: 0,
					duration: 1
					//               easing: 'ease-in'
				},
				y: {
					delay: 0,
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
						y: { set: 'Country' },
						color: { set: null },
						lightness: { set: 'Value 2 (+)' }
					},
					title: 'Heatmap / (?), (?), (sort???)',
					geometry: 'rectangle',
					legend: null,
					split: false
				}
			},
			{
				delay: 0,
				style: {
					delay: 0,
					duration: 0.25
				},
				geometry: {
					delay: 0,
					duration: 0.5
					//                easing: 'linear'
				},
				x: {
					delay: 0,
					duration: 0.5
					//               easing: 'ease-in'
				},
				y: {
					delay: 0,
					duration: 0.5
					//                easing: 'cubic-bezier(.39,0,.35,.99)'
				}
			}
		)
]

export default testSteps
