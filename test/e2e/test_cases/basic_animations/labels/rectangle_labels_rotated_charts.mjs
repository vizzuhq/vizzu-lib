import { data } from '../../../test_data/chart_types_eu.mjs'
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
				title: 'Rectangle Marker Label Positions - Top',
				angle: Math.PI / 18
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
				title: 'Rectangle Marker Label Positions - Left',
				angle: (Math.PI * 2) / 18
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
				title: 'Rectangle Marker Label Positions - Bottom',
				angle: (Math.PI * 3) / 18
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
				title: 'Rectangle Marker Label Positions - Right',
				angle: (Math.PI * 4) / 18
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
				title: 'Rectangle Marker Label Positions - Center/Horizontal',
				angle: (Math.PI * 5) / 18
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
				title: 'Rectangle Marker Label Positions - Center/Tangential',
				angle: (Math.PI * 6) / 18
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
				title: 'Rectangle Marker Label Positions - Center/Normal',
				angle: (Math.PI * 7) / 18
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
				title: 'Rectangle Marker Label Positions - Center/Vertical',
				angle: (Math.PI * 8) / 18
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
				title: 'Change Continuous',
				angle: (Math.PI * 9) / 18
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
				angle: (Math.PI * 27) / 18,
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
				title: 'Rectangle Marker Label Positions - Left',
				angle: (Math.PI * 28) / 18
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
				title: 'Rectangle Marker Label Positions - Bottom',
				angle: (Math.PI * 29) / 18
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
				title: 'Rectangle Marker Label Positions - Right',
				angle: (Math.PI * 30) / 18
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
				title: 'Rectangle Marker Label Positions - Center/Horizontal',
				angle: (Math.PI * 31) / 18
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
				title: 'Rectangle Marker Label Positions - Top/Normal',
				angle: (Math.PI * 32) / 18
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
				title: 'Rectangle Marker Label Positions - Top/Vertical',
				angle: (Math.PI * 33) / 18
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
				title: 'Rectangle Marker Label Positions - Top/Tangential',
				angle: (Math.PI * 34) / 18
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
				title: 'Rectangle Marker Label Positions - Top/Angle',
				angle: (Math.PI * 35) / 18
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
				angle: (Math.PI * 36) / 18,
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
				title: 'Rectangle Marker Label Positions - Center/Horizontal',
				angle: (Math.PI * 37) / 18
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
				title: 'Rectangle Marker Label Positions - Left',
				angle: (Math.PI * 38) / 18
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
				title: 'Rectangle Marker Label Positions - Bottom',
				angle: (Math.PI * 39) / 18
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
				title: 'Rectangle Marker Label Positions - Right',
				angle: (Math.PI * 40) / 18
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
				title: 'Rectangle Marker Label Positions - Center/Horizontal',
				angle: (Math.PI * 41) / 18
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
				title: 'Rectangle Marker Label Positions - Top/Normal',
				angle: (Math.PI * 42) / 18
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
				title: 'Rectangle Marker Label Positions - Top/Vertical',
				angle: (Math.PI * 6) / 18
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
				title: 'Rectangle Marker Label Positions - Top/Tangential',
				angle: (Math.PI * 16) / 18
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
				title: 'Rectangle Marker Label Positions - Top/Angle',
				angle: (Math.PI * 54) / 18
			},
			style: {
				plot: {
					marker: {
						label: {
							angle: 1 * 3.14
						}
					}
				}
			}
		})
]

export default testSteps
