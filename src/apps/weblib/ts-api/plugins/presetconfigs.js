export const presetConfigs = {
	column: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }] },
			label: { set: [{ name: 'y' }] }
		}
	},
	groupedColumn: {
		channels: {
			x: { set: [{ name: 'groupedBy' }, { name: 'x' }] },
			y: { set: [{ name: 'y' }] },
			label: { set: [{ name: 'y' }] },
			color: { set: [{ name: 'x' }] }
		}
	},
	stackedColumn: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }, { name: 'stackedBy' }] },
			label: { set: [{ name: 'y' }] },
			color: { set: [{ name: 'stackedBy' }] }
		}
	},
	splittedColumn: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }, { name: 'splittedBy' }], split: true },
			color: { set: [{ name: 'splittedBy' }] },
			label: { set: [{ name: 'y' }] }
		}
	},
	percentageColumn: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }, { name: 'stackedBy' }], align: 'stretch' },
			color: { set: [{ name: 'stackedBy' }] },
			label: { set: [{ name: 'y' }] }
		}
	},
	waterfall: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }, { name: 'x' }] },
			label: { set: [{ name: 'y' }] },
			color: { set: [{ name: 'y' }] }
		}
	},
	mekko: {
		channels: {
			x: { set: [{ name: 'x' }, { name: 'groupedBy' }] },
			y: { set: [{ name: 'y' }, { name: 'stackedBy' }] },
			color: { set: [{ name: 'stackedBy' }] },
			label: { set: [{ name: 'x' }] }
		}
	},
	marimekko: {
		channels: {
			x: { set: [{ name: 'x' }, { name: 'groupedBy' }] },
			y: { set: [{ name: 'y' }, { name: 'stackedBy' }], align: 'stretch' },
			color: { set: [{ name: 'stackedBy' }] },
			label: { set: [{ name: 'x' }] }
		}
	},
	bar: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }] },
			label: { set: [{ name: 'x' }] }
		}
	},
	groupedBar: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'groupedBy' }, { name: 'y' }] },
			label: { set: [{ name: 'x' }] },
			color: { set: [{ name: 'y' }] }
		}
	},
	stackedBar: {
		channels: {
			x: { set: [{ name: 'x' }, { name: 'stackedBy' }] },
			y: { set: [{ name: 'y' }] },
			label: { set: [{ name: 'x' }] },
			color: { set: [{ name: 'stackedBy' }] }
		}
	},
	splittedBar: {
		channels: {
			x: { set: [{ name: 'x' }, { name: 'splittedBy' }], split: true },
			y: { set: [{ name: 'y' }] },
			color: { set: [{ name: 'splittedBy' }] },
			label: { set: [{ name: 'x' }] }
		}
	},
	percentageBar: {
		channels: {
			x: { set: [{ name: 'x' }, { name: 'stackedBy' }], align: 'stretch' },
			y: { set: [{ name: 'y' }] },
			color: { set: [{ name: 'stackedBy' }] },
			label: { set: [{ name: 'x' }] }
		}
	},
	lollipop: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }] },
			label: { set: [{ name: 'y' }] }
		},
		geometry: 'circle'
	},
	scatter: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }] },
			noop: { set: [{ name: 'dividedBy' }] }
		},
		geometry: 'circle'
	},
	bubbleplot: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }] },
			color: { set: [{ name: 'color' }] },
			size: { set: [{ name: 'size' }] },
			label: { set: [{ name: 'dividedBy' }] },
			noop: { set: [{ name: 'dividedBy' }] }
		},
		geometry: 'circle'
	},
	area: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }] }
		},
		geometry: 'area'
	},
	stackedArea: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }, { name: 'stackedBy' }] },
			color: { set: [{ name: 'stackedBy' }] }
		},
		geometry: 'area'
	},
	percentageArea: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }, { name: 'stackedBy' }], align: 'stretch' },
			color: { set: [{ name: 'stackedBy' }] }
		},
		geometry: 'area'
	},
	splittedArea: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }, { name: 'splittedBy' }], split: true },
			color: { set: [{ name: 'splittedBy' }] }
		},
		geometry: 'area'
	},
	stream: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }, { name: 'stackedBy' }], align: 'center' },
			color: { set: [{ name: 'stackedBy' }] }
		},
		geometry: 'area'
	},
	verticalStream: {
		channels: {
			x: { set: [{ name: 'x' }, { name: 'stackedBy' }], align: 'center' },
			y: { set: [{ name: 'y' }] },
			color: { set: [{ name: 'stackedBy' }] }
		},
		geometry: 'area'
	},
	violin: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }, { name: 'splittedBy' }], align: 'center', split: true },
			color: { set: [{ name: 'splittedBy' }] }
		},
		geometry: 'area'
	},
	verticalViolin: {
		channels: {
			x: { set: [{ name: 'x' }, { name: 'splittedBy' }], align: 'center', split: true },
			y: { set: [{ name: 'y' }] },
			color: { set: [{ name: 'splittedBy' }] }
		},
		geometry: 'area'
	},
	line: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }] },
			color: { set: [{ name: 'dividedBy' }] }
		},
		geometry: 'line'
	},
	verticalLine: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }] },
			color: { set: [{ name: 'dividedBy' }] }
		},
		geometry: 'line'
	},
	pie: {
		channels: {
			x: { set: [{ name: 'angle' }, { name: 'by' }] },
			color: { set: [{ name: 'by' }] },
			label: { set: [{ name: 'angle' }] }
		},
		coordSystem: 'polar'
	},
	polarColumn: {
		channels: {
			x: { set: [{ name: 'angle' }] },
			y: { set: [{ name: 'radius' }] },
			label: { set: [{ name: 'radius' }] }
		},
		coordSystem: 'polar'
	},
	polarStackedColumn: {
		channels: {
			x: { set: [{ name: 'angle' }] },
			y: { set: [{ name: 'radius' }, { name: 'stackedBy' }] },
			color: { set: [{ name: 'stackedBy' }] }
		},
		coordSystem: 'polar'
	},
	variableRadiusPie: {
		channels: {
			x: { set: [{ name: 'angle' }, { name: 'by' }] },
			y: { set: [{ name: 'radius' }] },
			color: { set: [{ name: 'by' }] },
			label: { set: [{ name: 'radius' }] }
		},
		coordSystem: 'polar'
	},
	radialBar: {
		channels: {
			x: { set: [{ name: 'angle' }] },
			y: { set: [{ name: 'radius' }], range: { min: '-50%' } },
			label: { set: [{ name: 'angle' }] }
		},
		coordSystem: 'polar'
	},
	radialStackedBar: {
		channels: {
			x: { set: [{ name: 'angle' }, { name: 'stackedBy' }] },
			y: { set: [{ name: 'radius' }], range: { min: '-50%' } },
			color: { set: [{ name: 'stackedBy' }] },
			label: { set: [{ name: 'angle' }] }
		},
		coordSystem: 'polar'
	},
	donut: {
		channels: {
			x: { set: [{ name: 'angle' }, { name: 'stackedBy' }] },
			y: { range: { min: '-200%', max: '100%' } },
			color: { set: [{ name: 'stackedBy' }] }
		},
		coordSystem: 'polar'
	},
	nestedDonut: {
		channels: {
			x: { set: [{ name: 'angle' }, { name: 'stackedBy' }], align: 'stretch' },
			y: { set: [{ name: 'radius' }], range: { min: '-50%' } },
			color: { set: [{ name: 'stackedBy' }] },
			label: { set: [{ name: 'angle' }] }
		},
		coordSystem: 'polar'
	},
	polarScatter: {
		channels: {
			x: { set: [{ name: 'angle' }] },
			y: { set: [{ name: 'radius' }] },
			noop: { set: [{ name: 'dividedBy' }] }
		},
		coordSystem: 'polar',
		geometry: 'circle'
	},
	polarLine: {
		channels: {
			x: { set: [{ name: 'angle' }] },
			y: { set: [{ name: 'radius' }] },
			color: { set: [{ name: 'dividedBy' }] }
		},
		coordSystem: 'polar',
		geometry: 'line'
	},
	treemap: {
		channels: {
			size: { set: [{ name: 'size' }, { name: 'color' }] },
			color: { set: [{ name: 'color' }] },
			label: { set: [{ name: 'color' }] }
		}
	},
	stackedTreemap: {
		channels: {
			size: { set: [{ name: 'size' }, { name: 'dividedBy' }] },
			color: { set: [{ name: 'color' }] },
			label: { set: [{ name: 'dividedBy' }] },
			lightness: { set: [{ name: 'size' }] }
		}
	},
	heatmap: {
		channels: {
			x: { set: [{ name: 'x' }] },
			y: { set: [{ name: 'y' }] },
			lightness: { set: [{ name: 'lightness' }] }
		}
	},
	bubble: {
		channels: {
			size: { set: [{ name: 'size' }] },
			color: { set: [{ name: 'color' }] },
			label: { set: [{ name: 'color' }] }
		},
		geometry: 'circle'
	},
	stackedBubble: {
		channels: {
			size: { set: [{ name: 'size' }, { name: 'stackedBy' }] },
			color: { set: [{ name: 'color' }] }
		},
		geometry: 'circle'
	}
}
