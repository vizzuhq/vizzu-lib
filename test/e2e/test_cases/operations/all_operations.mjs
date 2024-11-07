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
				title: 'A simple column chart.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { attach: ['Categ. Child'] },
					color: { attach: ['Categ. Parent'] }
				},
				title: 'More categorical data series can be on the same channel.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { detach: ['Categ. Child'] },
					y: {
						set: ['Categ. Parent', 'Values 1'],
						range: { min: '0%', max: '110%' }
					},
					x: { set: ['Timeseries', 'Categ. Child'] },
					lightness: { set: ['Values 1'] }
				},
				title: 'Getting back to the stacked column chart in one step.'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							fontSize: 9,
							orientation: 'vertical',
							angle: 3.14 * -1
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: {
						set: ['Categ. Parent', 'Categ. Child', 'Values 1'],
						range: { min: '0%', max: '110%' }
					},
					x: { set: ['Timeseries'] },
					lightness: { set: ['Values 1'] }
				},
				title: 'Getting back to the stacked column chart in one step.'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'center',
							fontSize: null,
							orientation: null,
							angle: null
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Categ. Child'] },
					lightness: { set: null }
				},
				title: 'Getting back to the stacked column chart in one step.'
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
				align: 'stretch'
			}
		})
]

export default testSteps
