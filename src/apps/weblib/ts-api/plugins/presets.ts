import { Data, Config } from '../types/vizzu.js'

import { presetConfigs } from './presetconfigs.js'

interface PresetConfigs {
  [key: string]: Config.Chart
}

export default class Presets {
  _presetConfigs: PresetConfigs

  constructor() {
    this._presetConfigs = presetConfigs as PresetConfigs

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
