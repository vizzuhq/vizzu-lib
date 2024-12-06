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
				title: '1 Scatter plot',
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
					title: '2 Bubble plot / + Continuous(?)',
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
					title: '3 Scatter plot / - Continuous(?)',
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
		chart.animate({
			config: {
				channels: {
					x: { set: 'Value 1 (+)' },
					y: { set: 'Value 3 (+)' },
					noop: { set: 'Year' },
					size: { set: null },
					color: { set: 'Country' }
				},
				title: '5 Scatter plot / Drill down(?), + Discrete(?)',
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
					title: '6 Dot plot / Distribution(?) - Continuous(?)',
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
					title: '7 Dot plot / Distribution(?) - Continuous(?)',
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
					title: '8 Scatter plot / Drill down(?), + Discrete(?)',
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
				title: '9 Trellis Scatter / Components(?), Part-to-whole(?), Split(?)',
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
					title: '10 Scatter plot / Trellis off(?), Merge(?)',
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
				title: '11 Bubble plot / + Continuous(?)',
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
						size: { set: 'Value 2 (+)' },
						color: { set: null },
						label: { set: 'Value 2 (+)' }
					},
					title: '13 Bubble / Total(?), Aggregate(?), - Disc(?), - Conti(?)',
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
						x: { set: 'Value 1 (+)' },
						y: { set: 'Value 3 (+)' },
						noop: { set: 'Country' },
						size: { set: 'Value 2 (+)' },
						color: { set: 'Year' },
						label: null
					},
					title: '15 Bubble plot / + Continuous(?)',
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
					title: '16 Scatter plot / Merge(?), Aggregate(?), - Discrete(?)',
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
					title: '17 Scatter plot / Merge(?), Aggregate(?), - Discrete(?)',
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
					title: '18 Scatter plot / Merge(?), Aggregate(?), - Discrete(?)',
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
