export const presetConfigs = {
	column: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y'] },
			label: { set: ['y'] }
		}
	},
	groupedColumn: {
		channels: {
			x: { set: ['groupedBy', 'x'] },
			y: { set: ['y'] },
			label: { set: ['y'] },
			color: { set: ['x'] }
		}
	},
	stackedColumn: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y', 'stackedBy'] },
			label: { set: ['y'] },
			color: { set: ['stackedBy'] }
		}
	},
	splittedColumn: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y', 'splittedBy'] },
			color: { set: ['splittedBy'] },
			label: { set: ['y'] }
		},
		split: true
	},
	percentageColumn: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y', 'stackedBy'] },
			color: { set: ['stackedBy'] },
			label: { set: ['y'] }
		},
		align: 'stretch'
	},
	waterfall: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y', 'x'] },
			label: { set: ['y'] },
			color: { set: ['y'] }
		},
		align: 'stretch'
	},
	mekko: {
		channels: {
			x: { set: ['x', 'groupedBy'] },
			y: { set: ['y', 'stackedBy'] },
			color: { set: ['stackedBy'] },
			label: { set: ['groupedBy'] }
		}
	},
	marimekko: {
		channels: {
			x: { set: ['x', 'groupedBy'] },
			y: { set: ['y', 'stackedBy'] },
			color: { set: ['stackedBy'] },
			label: { set: ['groupedBy'] }
		},
		align: 'stretch'
	},
	bar: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y'] },
			label: { set: ['x'] }
		}
	},
	groupedBar: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['groupedBy', 'y'] },
			label: { set: ['x'] },
			color: { set: ['y'] }
		}
	},
	stackedBar: {
		channels: {
			x: { set: ['x', 'stackedBy'] },
			y: { set: ['y'] },
			label: { set: ['x'] },
			color: { set: ['stackedBy'] }
		}
	},
	splittedBar: {
		channels: {
			x: { set: ['x', 'splittedBy'] },
			y: { set: ['y'] },
			color: { set: ['splittedBy'] },
			label: { set: ['x'] }
		},
		split: true
	},
	percentageBar: {
		channels: {
			x: { set: ['x', 'stackedBy'] },
			y: { set: ['y'] },
			color: { set: ['stackedBy'] },
			label: { set: ['x'] }
		},
		align: 'stretch'
	},
	lollipop: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y'] },
			label: { set: ['y'] }
		},
		geometry: 'circle'
	},
	scatter: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y'] },
			noop: { set: ['dividedBy'] }
		},
		geometry: 'circle'
	},
	bubbleplot: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y'] },
			color: { set: ['color'] },
			size: { set: ['size'] },
			label: { set: ['dividedBy'] },
			noop: { set: ['dividedBy'] }
		},
		geometry: 'circle'
	},
	area: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y'] }
		},
		geometry: 'area'
	},
	stackedArea: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y', 'stackedBy'] },
			color: { set: ['stackedBy'] }
		},
		geometry: 'area'
	},
	percentageArea: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y', 'stackedBy'] },
			color: { set: ['stackedBy'] }
		},
		align: 'stretch',
		geometry: 'area'
	},
	splittedArea: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y', 'splittedBy'] },
			color: { set: ['splittedBy'] }
		},
		split: true,
		geometry: 'area'
	},
	stream: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y', 'stackedBy'] },
			color: { set: ['stackedBy'] }
		},
		geometry: 'area',
		align: 'center'
	},
	verticalStream: {
		channels: {
			x: { set: ['x', 'stackedBy'] },
			y: { set: ['y'] },
			color: { set: ['stackedBy'] }
		},
		geometry: 'area',
		align: 'center'
	},
	violin: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y', 'splittedBy'] },
			color: { set: ['splittedBy'] }
		},
		geometry: 'area',
		align: 'center',
		split: true
	},
	verticalViolin: {
		channels: {
			x: { set: ['x', 'splittedBy'] },
			y: { set: ['y'] },
			color: { set: ['splittedBy'] }
		},
		geometry: 'area',
		align: 'center',
		split: true
	},
	line: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y'] },
			color: { set: ['dividedBy'] }
		},
		geometry: 'line'
	},
	verticalLine: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y'] },
			color: { set: ['dividedBy'] }
		},
		geometry: 'line'
	},
	pie: {
		channels: {
			x: { set: ['angle', 'by'] },
			color: { set: ['by'] },
			label: { set: ['angle'] }
		},
		coordSystem: 'polar'
	},
	polarColumn: {
		channels: {
			x: { set: ['angle'] },
			y: { set: ['radius'] },
			label: { set: ['radius'] }
		},
		coordSystem: 'polar'
	},
	polarStackedColumn: {
		channels: {
			x: { set: ['angle'] },
			y: { set: ['radius', 'stackedBy'] },
			color: { set: ['stackedBy'] }
		},
		coordSystem: 'polar'
	},
	variableRadiusPie: {
		channels: {
			x: { set: ['angle', 'by'] },
			y: { set: ['radius'] },
			color: { set: ['by'] },
			label: { set: ['radius'] }
		},
		coordSystem: 'polar'
	},
	radialBar: {
		channels: {
			x: { set: ['angle'] },
			y: { set: ['radius'], range: { min: '-50%' } },
			label: { set: ['angle'] }
		},
		coordSystem: 'polar'
	},
	radialStackedBar: {
		channels: {
			x: { set: ['angle', 'stackedBy'] },
			y: { set: ['radius'], range: { min: '-50%' } },
			color: { set: ['stackedBy'] },
			label: { set: ['angle'] }
		},
		coordSystem: 'polar'
	},
	donut: {
		channels: {
			x: { set: ['angle', 'stackedBy'] },
			y: { range: { min: '-200%', max: '100%' } },
			color: { set: ['stackedBy'] }
		},
		coordSystem: 'polar'
	},
	nestedDonut: {
		channels: {
			x: { set: ['angle', 'stackedBy'] },
			y: { set: ['radius'], range: { min: '-50%' } },
			color: { set: ['stackedBy'] },
			label: { set: ['angle'] }
		},
		coordSystem: 'polar',
		align: 'stretch'
	},
	polarScatter: {
		channels: {
			x: { set: ['angle'] },
			y: { set: ['radius'] },
			noop: { set: ['dividedBy'] }
		},
		coordSystem: 'polar',
		geometry: 'circle'
	},
	polarLine: {
		channels: {
			x: { set: ['angle'] },
			y: { set: ['radius'] },
			color: { set: ['dividedBy'] }
		},
		coordSystem: 'polar',
		geometry: 'line'
	},
	treemap: {
		channels: {
			size: { set: ['size', 'color'] },
			color: { set: ['color'] },
			label: { set: ['color'] }
		}
	},
	stackedTreemap: {
		channels: {
			size: { set: ['size', 'dividedBy'] },
			color: { set: ['color'] },
			label: { set: ['dividedBy'] },
			lightness: { set: ['size'] }
		}
	},
	heatmap: {
		channels: {
			x: { set: ['x'] },
			y: { set: ['y'] },
			lightness: { set: ['lightness'] }
		}
	},
	bubble: {
		channels: {
			size: { set: ['size'] },
			color: { set: ['color'] },
			label: { set: ['color'] }
		},
		geometry: 'circle'
	},
	stackedBubble: {
		channels: {
			size: { set: ['size', 'stackedBy'] },
			color: { set: ['color'] }
		},
		geometry: 'circle'
	}
}
