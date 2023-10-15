import { Data, Config } from '../types/vizzu.js'

import { presetConfigs } from './presetconfigs.js'

type RawPresetConfigs = typeof presetConfigs

type PresetNames = keyof RawPresetConfigs

type PresetConfigs = {
  [Property in keyof RawPresetConfigs]: Config.Chart
}

type PresetInterface = {
  [Property in keyof RawPresetConfigs]: (config: Config.Chart) => Config.Chart
}

export default class Presets {
  private _presetConfigs: PresetConfigs

  constructor() {
    this._presetConfigs = presetConfigs as PresetConfigs

    for (const key in this._presetConfigs) {
      const name = key as PresetNames
      this._initPresetConfigChannels(this._presetConfigs[name]!.channels!)
      ;(this as unknown as PresetInterface)[name] = (config: Config.Chart): Config.Chart => {
        return this._buildPresetConfig(name, config)
      }
    }
  }

  private _initPresetConfigChannels(channels: Config.Channels): void {
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

  private _nullConfig(): Config.Chart {
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

  private _createPresetConfig(presetName: PresetNames): Config.Chart {
    const presetConfig = this._presetConfigs[presetName]
    const channelBase = { ...this._nullConfig().channels, ...presetConfig!.channels }
    const base = Object.assign(this._nullConfig(), presetConfig)
    base.channels = channelBase
    return base
  }

  private _getChannelCopy(channel: Data.SeriesList | null | undefined): Data.SeriesList | null {
    if (channel === null) return null
    if (channel === undefined) return null
    if (Array.isArray(channel)) return channel
    return [channel]
  }

  private _fillChannels(presetConfig: Config.Chart, config: Config.Chart | undefined): void {
    if (!config) return
    const channels = presetConfig.channels
    for (const key in channels) {
      const channelName = key as keyof Config.Channels
      const channel = channels[channelName]
      if (!this._isChannel(channel)) {
        continue
      } else if (typeof channel.set === 'string') {
        const key = channel.set as keyof Config.Chart
        channel.set = this._getChannelCopy(config[key] as Data.SeriesList)
      } else if (Array.isArray(channel.set)) {
        const newChannel = []
        for (let i = 0; i < channel.set.length; i++) {
          const key = channel.set[i]! as keyof Config.Chart
          const channelConfig = this._getChannelCopy(config[key] as Data.SeriesList)
          if (channelConfig !== null) {
            newChannel.push(channelConfig)
          }
        }
        channel.set = newChannel.length > 0 ? newChannel.flat() : null
      }
    }
  }

  private _isChannel(
    channel: Config.Channel | null | undefined | Data.SeriesList
  ): channel is Config.Channel {
    return typeof channel === 'object' && channel !== null && 'set' in channel
  }

  private _setupUserParams(base: Config.Chart, config: Config.Chart): void {
    ;['legend', 'title', 'subtitle', 'caption', 'reverse', 'sort'].forEach((key) => {
      const prop = key as keyof Config.Chart
      if (config[prop] !== undefined) {
        base[prop] = config[prop] as undefined
      }
    })
  }

  private _buildPresetConfig(presetName: PresetNames, config: Config.Chart): Config.Chart {
    const presetConfig = this._createPresetConfig(presetName)
    this._fillChannels(presetConfig, config)
    this._setupUserParams(presetConfig, config)
    return presetConfig
  }
}
