import { data } from '../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Timeseries'] },
					y: { attach: ['Values 1'], range: { min: '0%', max: '110%' } },
					label: { attach: ['Values 1'] }
				},
				title: 'A simple column chart.',
				legend: null
			},
			style: {
				fontSize: 12
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { attach: ['Categ. Parent'] },
					color: { attach: ['Categ. Parent'] }
				},
				title: 'A cross-metric added to the y-axis and the color channel.',
				legend: 'color'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {},
				title: 'The geometric elements used can be changed to area...',
				geometry: 'area'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Categ. Parent'] }
				},
				title: '...or line...',
				geometry: 'line'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { attach: ['Categ. Parent'] }
				},
				title: '...or back to rectangle.',
				geometry: 'rectangle'
			}
		}),
	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						y: { detach: ['Values 1'] },
						x: { attach: ['Values 1'], range: { min: '0%', max: '110%' } }
					},
					legend: null,
					title: 'Values can be grouped by the other axis too.'
				}
			},
			{ x: { easing: 'ease-in' } }
		),
	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { detach: 'Timeseries' }
					},
					title: 'Values can be grouped by the other axis too.'
				}
			},
			{ x: { easing: 'ease-out' } }
		),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { range: { min: '0%', max: '133.3%' } }
				},
				title: 'Using polar coordinates instead of cartesian is also an option.',
				coordSystem: 'polar',
				legend: null
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { attach: 'Timeseries' },
					lightness: { attach: ['Values 2'] }
				},
				title: 'Values can be added to the lightness channel.',
				legend: 'lightness'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Categ. Parent'] },
					x: {
						detach: ['Values 1', 'Timeseries'],
						range: { min: 'auto', max: 'auto' }
					},
					size: { attach: ['Values 3', 'Timeseries'] }
				},
				title: 'Charts can also exist without any data on the axes.',
				legend: null,
				coordSystem: 'cartesian'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { attach: ['Categ. Child'] }
				},
				title: 'More categorical data series can be on the same channel.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { detach: ['Values 3', 'Timeseries', 'Categ. Child'] },
					y: {
						attach: ['Categ. Parent', 'Categ. Child', 'Values 1'],
						range: { min: '0%', max: '110%' }
					},
					x: { attach: ['Timeseries'] },
					lightness: { detach: ['Values 2'] }
				},
				legend: 'color',
				title: 'Getting back to the stacked bar chart in one step.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Categ. Child'] }
				},
				title: 'Getting back to the stacked bar chart in one step.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Categ. Parent'] },
					x: { attach: ['Categ. Parent'] }
				},
				title: 'Data can be grouped...'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { detach: ['Categ. Parent'] },
					y: { attach: ['Categ. Parent'] }
				},
				title: '...or stacked, by putting a dimension from one axis to the other.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Comparing ratios is just another option out of many more...',
				y: { align: 'stretch' }
			}
		})
]

export default testSteps
