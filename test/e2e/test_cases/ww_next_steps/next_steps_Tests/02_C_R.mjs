import { data_8 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,
			config: {
				channels: {
					x: { set: 'Country' },
					y: { set: 'Value 2 (+)' },
					label: { set: 'Value 2 (+)' }
				},
				title: '1 Column',
				geometry: 'rectangle',
				orientation: 'horizontal',
				legend: null
			},
			style: {
				plot: {
					xAxis: {
						label: {
							angle: '2.5'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Country' },
					y: { set: ['Value 2 (+)', 'Joy factors'] },
					color: { set: 'Joy factors' },
					label: { set: 'Value 2 (+)' }
				},
				title: '2 Stacked Column / Drill down(?), + Discrete(?)',
				geometry: 'rectangle',
				orientation: 'horizontal',
				legend: 'color'
			},
			style: {
				plot: {
					marker: {
						label: {
							fontSize: 9
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Country' },
					y: { set: 'Value 2 (+)' },
					color: { set: null },
					label: { set: 'Value 2 (+)' }
				},
				title: '3 Column / Aggregate(?), - Discrete(?)',
				geometry: 'rectangle',
				orientation: 'horizontal',
				legend: null
			},
			style: {
				plot: {
					marker: {
						label: {
							fontSize: null
						}
					}
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: null },
					y: { set: 'Value 2 (+)' },
					color: { set: null },
					label: { set: 'Value 2 (+)' }
				},
				title: '4 Stacked Column / Total(?), Aggregate(?), - Discrete(?), Total(?)',
				geometry: 'rectangle',
				orientation: 'horizontal',
				legend: null
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: null },
					y: { set: ['Country', 'Value 2 (+)'] },
					color: { set: null },
					label: { set: null }
				},
				title: '5 Stacked Column / Drill down(?), + Discrete(?)',
				geometry: 'rectangle',
				orientation: 'horizontal',
				legend: null
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Country' },
					y: { set: 'Value 2 (+)' },
					color: { set: null },
					label: { set: 'Value 2 (+)' }
				},
				title: '6 Column - Group(?)',
				geometry: 'rectangle',
				orientation: 'horizontal',
				legend: null
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Value 5 (+/-)' },
					y: { set: 'Value 2 (+)' },
					color: { set: 'Country' },
					label: { set: null }
				},
				title: '7 Bubble plot / + Continuous(?)',
				geometry: 'circle',
				orientation: 'horizontal',
				legend: 'color'
			},
			style: {
				plot: {
					xAxis: {
						label: {
							angle: '3.14'
						}
					}
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Country',
					y: 'Value 2 (+)',
					color: null,
					label: 'Value 2 (+)'
				},
				title: '8 Column / - Continuous(?)',
				geometry: 'rectangle',
				orientation: 'horizontal',
				legend: null
			},
			style: {
				plot: {
					xAxis: {
						label: {
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
					x: { set: ['Value 2 (+)'], range: { min: '0%', max: '110%' } },
					y: null,
					color: null,
					label: 'Value 2 (+)'
				},
				title: '9 Bar / Total(?)',
				geometry: 'rectangle',
				orientation: 'vertical',
				legend: null
			}
		}),

	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
