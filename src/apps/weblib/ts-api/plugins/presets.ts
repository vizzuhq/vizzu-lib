/// <reference types="../types/vizzu.d.ts" />

interface PresetConfigs {
  [key: string]: Config.Chart
}

export default class Presets {
  _presetConfigs: PresetConfigs

  constructor() {
    this._presetConfigs = {
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

    for (const key in this._presetConfigs) {
      this._initPresetConfigChannels(this._presetConfigs[key]!.channels!)
      ;(this as any)[key] = (config: Config.Chart) => {
        return this._buildPresetConfig(key, config)
      }
    }
  }

  _initPresetConfigChannels(channels: Config.Channels) {
    for (const key in channels) {
      const channelName = key as keyof Config.Channels
      const channel = channels[channelName]!
      if (typeof channel !== 'object' || Array.isArray(channel)) {
        channels[channelName] = {
          set: channels[channelName]
        } as Config.Channel
      }
    }
  }

  _nullConfig() {
    return {
      align: 'none',
      coordSystem: 'cartesian',
      orientation: 'auto',
      rotate: 0,
      split: false,
      geometry: 'rectangle',
      channels: {
        x: null,
        y: null,
        color: null,
        lightness: null,
        size: null,
        noop: null,
        label: null
      }
    }
  }

  _createPresetConfig(presetName: string) {
    const presetConfig = this._presetConfigs[presetName]
    const nullConfig = this._nullConfig()
    const channelBase = Object.assign(nullConfig.channels, presetConfig!.channels)
    const base = Object.assign(this._nullConfig(), presetConfig)
    base.channels = channelBase
    return base
  }

  _getChannelCopy(channel: Data.SeriesList | null | undefined) {
    if (channel === null) return null
    if (channel === undefined) return null
    if (Array.isArray(channel)) return channel
    return [channel]
  }

  _fillChannels(presetConfig: Config.Chart, config: Config.Chart | undefined) {
    if (!config) return
    const channels = presetConfig.channels
    for (const key in channels) {
      const channelName = key as keyof Config.Channels
      const channel = channels[channelName]
      if (!this._isChannel(channel)) {
        continue
      } else if (typeof channel.set === 'string') {
        channel.set = this._getChannelCopy((config as any)[channel.set])
      } else if (Array.isArray(channel.set)) {
        const newChannel = []
        for (let i = 0; i < channel.set.length; i++) {
          const channelConfig = this._getChannelCopy((config as any)[channel.set[i]!])
          if (channelConfig !== null) {
            newChannel.push(channelConfig)
          }
        }
        channel.set = newChannel.length > 0 ? newChannel.flat() : null
      }
    }
  }

  _isChannel(
    channel: Config.Channel | null | undefined | Data.SeriesList
  ): channel is Config.Channel {
    return typeof channel === 'object' && channel !== null && 'set' in channel
  }

  _setupUserParams(base: Config.Chart, config: Config.Chart) {
    ;['legend', 'title', 'subtitle', 'caption', 'reverse', 'sort'].forEach((key) => {
      const prop = key as keyof Config.Chart
      if (config[prop] !== undefined) {
        ;(base as any)[prop] = config[prop]
      }
    })
  }

  _buildPresetConfig(presetName: string, config: Config.Chart): Config.Chart {
    const presetConfig = this._createPresetConfig(presetName)
    this._fillChannels(presetConfig, config)
    this._setupUserParams(presetConfig, config)
    return presetConfig
  }
}
