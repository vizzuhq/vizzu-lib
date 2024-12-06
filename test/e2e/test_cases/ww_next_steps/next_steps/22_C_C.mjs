import { data_6 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: {
				channels: {
					x: { set: 'Value 1 (+)' },
					y: { set: 'Value 3 (+)' },
					noop: { set: ['Year'] }
				},
				title: 'Scatter plot',
				legend: null,
				geometry: 'circle',
				coordSystem: 'cartesian'
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
						x: { set: 'Value 1 (+)' },
						y: { set: 'Value 3 (+)' },
						size: { set: 'Value 2 (+)' },
						noop: { set: 'Year' }
					},
					title: 'Bubble plot / + Continuous(?)',
					legend: null,
					geometry: 'circle',
					coordSystem: 'cartesian'
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
						x: { set: 'Value 1 (+)' },
						y: { set: 'Value 3 (+)' },
						size: { set: null },
						noop: { set: 'Year' }
					},
					title: 'Scatter plot / - Continuous(?)',
					legend: null,
					geometry: 'circle',
					coordSystem: 'cartesian'
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
						x: { set: 'Value 1 (+)' },
						y: { set: 'Value 3 (+)' },
						noop: { set: 'Year' },
						size: { set: 'Country' }
					},
					title: 'Scatter plot / Drill down(?), + Discrete(?)',
					legend: null,
					geometry: 'circle',
					coordSystem: 'cartesian'
				}
			},
			{
				delay: 0,
				duration: 0.5
			}
		),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Value 1 (+)' },
					y: { set: 'Value 3 (+)' },
					noop: { set: 'Year' },
					size: { set: null },
					color: { set: 'Country' }
				},
				title: 'Scatter plot / Drill down(?), + Discrete(?)',
				legend: 'color',
				geometry: 'circle',
				coordSystem: 'cartesian'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: 'Value 3 (+)' },
						noop: { set: 'Year' },
						size: { set: null },
						color: { set: 'Country' }
					},
					title: 'Dot plot / Distribution(?) - Continuous(?)',
					legend: 'color',
					geometry: 'circle',
					coordSystem: 'cartesian'
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
						x: { set: 'Value 1 (+)' },
						y: { set: 'Country' },
						noop: { set: 'Year' },
						size: { set: null },
						color: { set: 'Country' }
					},
					title: 'Dot plot / Distribution(?) - Continuous(?)',
					legend: 'color',
					geometry: 'circle',
					coordSystem: 'cartesian'
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
						x: { set: 'Value 1 (+)' },
						y: { set: 'Value 3 (+)' },
						noop: { set: 'Year' },
						size: { set: null },
						color: { set: 'Country' }
					},
					title: 'Scatter plot / Drill down(?), + Discrete(?)',
					legend: 'color',
					geometry: 'circle',
					coordSystem: 'cartesian'
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
					x: { set: ['Value 1 (+)', 'Country'], split: true },
					y: { set: 'Value 3 (+)' },
					noop: { set: 'Year' },
					color: { set: 'Country' }
				},
				title: 'Trellis Scatter / Components(?), Part-to-whole(?), Split(?)',
				legend: 'color',
				geometry: 'circle',
				coordSystem: 'cartesian',
				orientation: 'vertical'
			}
		}),
	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Value 1 (+)' },
						y: { set: 'Value 3 (+)' },
						noop: { set: 'Year' },
						color: { set: 'Country' }
					},
					title: 'Scatter plot / Trellis off(?), Merge(?)',
					legend: 'color',
					geometry: 'circle',
					coordSystem: 'cartesian'
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
					x: { set: 'Value 1 (+)' },
					y: { set: 'Value 3 (+)' },
					noop: { set: 'Year' },
					size: { set: 'Value 2 (+)' },
					color: { set: 'Country' }
				},
				title: 'Bubble plot / + Continuous(?)',
				legend: 'color',
				geometry: 'circle',
				coordSystem: 'cartesian'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: null },
						y: { set: null },
						noop: { set: null },
						size: { set: ['Country', 'Year', 'Value 2 (+)'] },
						color: { set: 'Country' }
					},
					title: 'Bubble / Aggregate(?), - Disc(?), - Conti(?)',
					legend: null,
					geometry: 'circle',
					coordSystem: 'cartesian'
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
						x: { set: null },
						y: { set: null },
						noop: { set: null },
						size: { set: 'Value 2 (+)' },
						color: { set: null },
						label: { set: 'Value 2 (+)' }
					},
					title: 'Bubble / Total(?), Aggregate(?), - Disc(?), - Conti(?)',
					legend: null,
					geometry: 'circle',
					coordSystem: 'cartesian'
				},
				style: {
					plot: {
						marker: {
							label: {
								fontSize: '1.5em'
							}
						}
					}
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
						y: { set: null },
						noop: { set: null },
						size: { set: ['Country', 'Year', 'Value 2 (+)'] },
						color: { set: null },
						label: { set: null }
					},
					title: 'Bubble / Drill down(?), + Disc(?)',
					legend: null,
					geometry: 'circle',
					coordSystem: 'cartesian'
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
						x: { set: 'Value 1 (+)' },
						y: { set: 'Value 3 (+)' },
						noop: { set: 'Country' },
						size: { set: 'Value 2 (+)' },
						color: { set: 'Year' }
					},
					title: 'Bubble plot / + Continuous(?)',
					legend: 'color',
					geometry: 'circle',
					coordSystem: 'cartesian'
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
						x: { set: 'Value 1 (+)', split: false },
						y: { set: 'Value 3 (+)' },
						noop: { set: 'Year' },
						size: { set: ['Country', 'Value 2 (+)'] },
						color: { set: 'Year' }
					},
					title: 'Scatter plot / Merge(?), Aggregate(?), - Discrete(?)',
					legend: 'color',
					geometry: 'circle',
					coordSystem: 'cartesian'
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
						x: { set: 'Value 1 (+)' },
						y: { set: 'Value 3 (+)' },
						size: { set: 'Country' },
						noop: { set: 'Year' }
					},
					title: 'Scatter plot / Merge(?), Aggregate(?), - Discrete(?)',
					legend: null,
					geometry: 'circle',
					coordSystem: 'cartesian'
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
						x: { set: 'Value 1 (+)' },
						y: { set: 'Value 3 (+)' },
						size: { set: null },
						noop: { set: 'Year' }
					},
					title: 'Scatter plot / Merge(?), Aggregate(?), - Discrete(?)',
					legend: null,
					geometry: 'circle',
					coordSystem: 'cartesian'
				}
			},
			{
				delay: 0
			}
		),

	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
