import { data } from '../../../../test_data/chart_types_eu.mjs'
data.filter = (record) =>
	record.Country !== 'Germany' &&
	record.Country !== 'Italy' &&
	record.Country !== 'Hungary' &&
	record.Country !== 'Netherlands' &&
	record.Country !== 'Estonia' &&
	record.Country !== 'Belgium' &&
	record.Country !== 'Sweden' &&
	record.Country !== 'Poland' &&
	record.Country !== 'Malta' &&
	record.Country !== 'Romania' &&
	record.Country !== 'Greece' &&
	record.Country !== 'Slovakia' &&
	record.Country !== 'Ireland' &&
	record.Country !== 'Lithuania' &&
	record.Country !== 'Croatia' &&
	record.Country !== 'Slovenia' &&
	record.Country !== 'Portugal' &&
	record.Country !== 'Finland' &&
	record.Country !== 'United Kingdom'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
					x: { attach: ['Country'] },
					color: { attach: ['Country'] },
					label: { attach: ['Value 5 (+/-)'] }
				},
				title: 'Rectangle Marker Label Positions - Center'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Top'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							orientation: 'horizontal'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Left'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'left'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Bottom'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'bottom'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Right'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'right'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Center/Horizontal'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'center'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Center/Tangential'
			},
			style: {
				plot: {
					marker: {
						label: {
							orientation: 'tangential'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Center/Normal'
			},
			style: {
				plot: {
					marker: {
						label: {
							orientation: 'normal'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Center/Vertical'
			},
			style: {
				plot: {
					marker: {
						label: {
							orientation: 'vertical'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { attach: ['Value 2 (+)'] },
					label: { attach: ['Value 2 (+)'] }
				},
				title: 'Change Continuous'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							orientation: 'horizontal'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Value 2 (+)'], range: { min: 'auto', max: 'auto' } },
					x: { attach: ['Value 2 (+)'] },
					label: { attach: ['Value 2 (+)', 'Country'] }
				},
				title: 'Polar Coordinate',
				coordSystem: 'polar'
			},
			style: {
				plot: {
					marker: {
						label: {
							orientation: 'horizontal'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Left'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'left'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Bottom'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'bottom'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Right'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'right'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Center/Horizontal'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'center',
							orientation: 'horizontal'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Top/Normal'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							orientation: 'normal'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Top/Vertical'
			},
			style: {
				plot: {
					marker: {
						label: {
							orientation: 'vertical'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: '1 Rectangle Marker Label Positions - Top/Tangential'
			},
			style: {
				plot: {
					marker: {
						label: {
							orientation: 'tangential'
						}
					}
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				title: '2 Rectangle Marker Label Positions - Top/Tangential'
			},
			style: {
				plot: {
					marker: {
						label: {
							paddingLeft: '100',
							orientation: 'tangential'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Top/Angle'
			},
			style: {
				plot: {
					marker: {
						label: {
							angle: 0.5 * 3.14
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { detach: ['Value 2 (+)', 'Country'] },
					size: { attach: ['Value 2 (+)'] }
				},
				title: 'Without Coordinate',
				coordSystem: 'cartesian'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'center',
							orientation: 'horizontal'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Center/Horizontal'
			},
			style: {
				plot: {
					marker: {
						label: {
							orientation: 'horizontal',
							angle: null
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							orientation: 'horizontal'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Left'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'left'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Bottom'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'bottom'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Right'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'right'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Center/Horizontal'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'center',
							orientation: 'horizontal'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Top/Normal'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							orientation: 'normal'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Top/Vertical'
			},
			style: {
				plot: {
					marker: {
						label: {
							orientation: 'vertical'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Top/Tangential'
			},
			style: {
				plot: {
					marker: {
						label: {
							orientation: 'tangential'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Positions - Top/Angle'
			},
			style: {
				plot: {
					marker: {
						label: {
							angle: 0.5 * 3.14
						}
					}
				}
			}
		})
]

export default testSteps
