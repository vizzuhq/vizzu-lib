import { Snapshot } from '../module/cchart.js'
import { CAnimation } from '../module/canimctrl.js'
export class Shorthands {
  constructor() {
    this.meta = {
      name: 'shorthands'
    }
  }
  get hooks() {
    const hooks = {
      prepareAnimation: Object.assign(
        (ctx, next) => {
          this._normalize(ctx)
          next()
        },
        { priority: 1 }
      )
    }
    return hooks
  }
  register(chart) {
    this._chart = chart
    this._channelNames = Object.keys(this._chart.config.channels)
  }
  _normalize(ctx) {
    if (!(ctx.target instanceof CAnimation)) {
      const keyframes = this._isKeyframes(ctx.target) ? ctx.target : [ctx.target]
      ctx.target = keyframes
        .map((keyframe) => (this._isKeyframe(keyframe) ? keyframe : { target: keyframe }))
        .map((keyframe) => this._normalizeKeyframe(keyframe))
    }
    const options = this._normalizeOptions(ctx.options)
    if (options) ctx.options = options
  }
  _isKeyframes(value) {
    return Array.isArray(value)
  }
  _isKeyframe(value) {
    return 'target' in value
  }
  _normalizeKeyframe(keyframe) {
    const options = this._normalizeOptions(keyframe.options)
    return Object.assign(
      {
        target: this._normalizeTarget(keyframe.target)
      },
      options ? { options } : {}
    )
  }
  _normalizeTarget(target) {
    if (target && !(target instanceof Snapshot)) {
      if (this._isConfig(target)) {
        target = { config: target }
      }
      if (target.style === null) {
        target.style = { '': null }
      }
      target.style = target.style || {}
      const cfg = this._normalizeConfig(target.config)
      if (cfg) target.config = cfg
    }
    return target
  }
  _isConfig(value) {
    return !('data' in value || 'style' in value || 'config' in value)
  }
  _normalizeConfig(config) {
    if (config !== null && typeof config === 'object') {
      Object.keys(config).forEach((key) => {
        if (this._isChannelName(key)) {
          config.channels = config.channels || {}
          config.channels[key] = config[key]
          delete config[key]
        }
      })
    }
    if (config?.channels) {
      config.channels = this._normalizeChannels(config.channels)
    }
    return config
  }
  _isChannelName(value) {
    return this._channelNames.includes(value)
  }
  _normalizeChannels(channels) {
    Object.keys(channels).forEach((key) => {
      const ch = key
      channels[ch] = this._normalizeChannel(channels[ch])
    })
    return channels
  }
  _normalizeChannel(channel) {
    if (typeof channel === 'string') {
      channel = [channel]
    }
    if (channel === null || Array.isArray(channel)) {
      channel = { set: channel }
    }
    if (typeof channel?.attach === 'string') {
      channel.attach = [channel.attach]
    }
    if (typeof channel?.detach === 'string') {
      channel.detach = [channel.detach]
    }
    if (typeof channel?.set === 'string') {
      channel.set = [channel.set]
    }
    if (Array.isArray(channel?.set) && channel?.set.length === 0) {
      channel.set = null
    }
    return channel
  }
  _normalizeOptions(options) {
    if (typeof options !== 'undefined') {
      if (options === null) {
        options = { duration: 0 }
      } else if (typeof options === 'string' || typeof options === 'number') {
        options = { duration: options }
      }
    }
    return options
  }
}
