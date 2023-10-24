export const presetConfigs = {
  column: {
    channels: {
      x: 'x',
      y: 'y',
      label: 'y'
    }
  },
  groupedColumn: {
    channels: {
      x: ['groupedBy', 'x'],
      y: 'y',
      label: 'y',
      color: 'x'
    }
  },
  stackedColumn: {
    channels: {
      x: 'x',
      y: ['y', 'stackedBy'],
      label: 'y',
      color: 'stackedBy'
    }
  },
  splittedColumn: {
    channels: {
      x: 'x',
      y: ['y', 'splittedBy'],
      color: 'splittedBy',
      label: 'y'
    },
    split: true
  },
  percentageColumn: {
    channels: {
      x: 'x',
      y: ['y', 'stackedBy'],
      color: 'stackedBy',
      label: 'y'
    },
    align: 'stretch'
  },
  waterfall: {
    channels: {
      x: 'x',
      y: ['y', 'x'],
      label: 'y',
      color: 'y'
    },
    align: 'stretch'
  },
  mekko: {
    channels: {
      x: ['x', 'groupedBy'],
      y: ['y', 'stackedBy'],
      color: 'stackedBy',
      label: 'groupedBy'
    }
  },
  marimekko: {
    channels: {
      x: ['x', 'groupedBy'],
      y: ['y', 'stackedBy'],
      color: 'stackedBy',
      label: 'groupedBy'
    },
    align: 'stretch'
  },
  bar: {
    channels: {
      x: 'x',
      y: 'y',
      label: 'x'
    }
  },
  groupedBar: {
    channels: {
      x: 'x',
      y: ['groupedBy', 'y'],
      label: 'x',
      color: 'y'
    }
  },
  stackedBar: {
    channels: {
      x: ['x', 'stackedBy'],
      y: 'y',
      label: 'x',
      color: 'stackedBy'
    }
  },
  splittedBar: {
    channels: {
      x: ['x', 'splittedBy'],
      y: 'y',
      color: 'splittedBy',
      label: 'x'
    },
    split: true
  },
  percentageBar: {
    channels: {
      x: ['x', 'stackedBy'],
      y: 'y',
      color: 'stackedBy',
      label: 'x'
    },
    align: 'stretch'
  },
  lollipop: {
    channels: {
      x: 'x',
      y: 'y',
      label: 'y'
    },
    geometry: 'circle'
  },
  scatter: {
    channels: {
      x: 'x',
      y: 'y',
      noop: 'dividedBy'
    },
    geometry: 'circle'
  },
  bubbleplot: {
    channels: {
      x: 'x',
      y: 'y',
      color: 'color',
      size: 'size',
      label: 'dividedBy',
      noop: 'dividedBy'
    },
    geometry: 'circle'
  },
  area: {
    channels: {
      x: 'x',
      y: 'y'
    },
    geometry: 'area'
  },
  stackedArea: {
    channels: {
      x: 'x',
      y: ['y', 'stackedBy'],
      color: 'stackedBy'
    },
    geometry: 'area'
  },
  percentageArea: {
    channels: {
      x: 'x',
      y: ['y', 'stackedBy'],
      color: 'stackedBy'
    },
    align: 'stretch',
    geometry: 'area'
  },
  splittedArea: {
    channels: {
      x: 'x',
      y: ['y', 'splittedBy'],
      color: 'splittedBy'
    },
    split: true,
    geometry: 'area'
  },
  stream: {
    channels: {
      x: 'x',
      y: ['y', 'stackedBy'],
      color: 'stackedBy'
    },
    geometry: 'area',
    align: 'center'
  },
  verticalStream: {
    channels: {
      x: ['x', 'stackedBy'],
      y: 'y',
      color: 'stackedBy'
    },
    geometry: 'area',
    align: 'center'
  },
  violin: {
    channels: {
      x: 'x',
      y: ['y', 'splittedBy'],
      color: 'splittedBy'
    },
    geometry: 'area',
    align: 'center',
    split: true
  },
  verticalViolin: {
    channels: {
      x: ['x', 'splittedBy'],
      y: 'y',
      color: 'splittedBy'
    },
    geometry: 'area',
    align: 'center',
    split: true
  },
  line: {
    channels: {
      x: 'x',
      y: 'y',
      color: 'dividedBy'
    },
    geometry: 'line'
  },
  verticalLine: {
    channels: {
      x: 'x',
      y: 'y',
      color: 'dividedBy'
    },
    geometry: 'line'
  },
  pie: {
    channels: {
      x: ['angle', 'by'],
      color: 'by',
      label: 'angle'
    },
    coordSystem: 'polar'
  },
  polarColumn: {
    channels: {
      x: 'angle',
      y: 'radius',
      label: 'radius'
    },
    coordSystem: 'polar'
  },
  polarStackedColumn: {
    channels: {
      x: 'angle',
      y: ['radius', 'stackedBy'],
      color: 'stackedBy'
    },
    coordSystem: 'polar'
  },
  variableRadiusPie: {
    channels: {
      x: ['angle', 'by'],
      y: 'radius',
      color: 'by',
      label: 'radius'
    },
    coordSystem: 'polar'
  },
  radialBar: {
    channels: {
      x: 'angle',
      y: { set: 'radius', range: { min: '-50%' } },
      label: 'angle'
    },
    coordSystem: 'polar'
  },
  radialStackedBar: {
    channels: {
      x: ['angle', 'stackedBy'],
      y: { set: 'radius', range: { min: '-50%' } },
      color: 'stackedBy',
      label: 'angle'
    },
    coordSystem: 'polar'
  },
  donut: {
    channels: {
      x: ['angle', 'stackedBy'],
      y: { range: { min: '-200%', max: '100%' } },
      color: 'stackedBy'
    },
    coordSystem: 'polar'
  },
  nestedDonut: {
    channels: {
      x: ['angle', 'stackedBy'],
      y: { set: 'radius', range: { min: '-50%' } },
      color: 'stackedBy',
      label: 'angle'
    },
    coordSystem: 'polar',
    align: 'stretch'
  },
  polarScatter: {
    channels: {
      x: 'angle',
      y: 'radius',
      noop: 'dividedBy'
    },
    coordSystem: 'polar',
    geometry: 'circle'
  },
  polarLine: {
    channels: {
      x: 'angle',
      y: 'radius',
      color: 'dividedBy'
    },
    coordSystem: 'polar',
    geometry: 'line'
  },
  treemap: {
    channels: {
      size: ['size', 'color'],
      color: 'color',
      label: 'color'
    }
  },
  stackedTreemap: {
    channels: {
      size: ['size', 'dividedBy'],
      color: 'color',
      label: 'dividedBy',
      lightness: 'size'
    }
  },
  heatmap: {
    channels: {
      x: 'x',
      y: 'y',
      lightness: 'lightness'
    }
  },
  bubble: {
    channels: {
      size: 'size',
      color: 'color',
      label: 'color'
    },
    geometry: 'circle'
  },
  stackedBubble: {
    channels: {
      size: ['size', 'stackedBy'],
      color: 'color'
    },
    geometry: 'circle'
  }
}
