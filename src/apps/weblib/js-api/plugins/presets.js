import { presetConfigs } from './presetconfigs.js'
export default class Presets {
  constructor() {
    this._presetConfigs = presetConfigs
    for (const key in this._presetConfigs) {
      const name = key
      this._initPresetConfigChannels(this._presetConfigs[name].channels)
      this[name] = (config) => {
        return this._buildPresetConfig(name, config)
      }
    }
  }
  _initPresetConfigChannels(channels) {
    for (const key in channels) {
      const channelName = key
      const channel = channels[channelName]
      if (typeof channel !== 'object' || Array.isArray(channel)) {
        channels[channelName] = {
          set: channels[channelName]
        }
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
  _createPresetConfig(presetName) {
    const presetConfig = this._presetConfigs[presetName]
    const channelBase = { ...this._nullConfig().channels, ...presetConfig.channels }
    const base = Object.assign(this._nullConfig(), presetConfig)
    base.channels = channelBase
    return base
  }
  _getChannelCopy(channel) {
    if (channel === null) return null
    if (channel === undefined) return null
    if (Array.isArray(channel)) return channel
    return [channel]
  }
  _fillChannels(presetConfig, config) {
    if (!config) return
    const channels = presetConfig.channels
    for (const key in channels) {
      const channelName = key
      const channel = channels[channelName]
      if (!this._isChannel(channel)) {
        continue
      } else if (typeof channel.set === 'string') {
        const key = channel.set
        channel.set = this._getChannelCopy(config[key])
      } else if (Array.isArray(channel.set)) {
        const newChannel = []
        for (let i = 0; i < channel.set.length; i++) {
          const key = channel.set[i]
          const channelConfig = this._getChannelCopy(config[key])
          if (channelConfig !== null) {
            newChannel.push(channelConfig)
          }
        }
        channel.set = newChannel.length > 0 ? newChannel.flat() : null
      }
    }
  }
  _isChannel(channel) {
    return typeof channel === 'object' && channel !== null && 'set' in channel
  }
  _setupUserParams(base, config) {
    ;['legend', 'title', 'subtitle', 'caption', 'reverse', 'sort'].forEach((key) => {
      const prop = key
      if (config[prop] !== undefined) {
        base[prop] = config[prop]
      }
    })
  }
  _buildPresetConfig(presetName, config) {
    const presetConfig = this._createPresetConfig(presetName)
    this._fillChannels(presetConfig, config)
    this._setupUserParams(presetConfig, config)
    return presetConfig
  }
}
