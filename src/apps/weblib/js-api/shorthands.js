import { CObject } from './objregistry.js'
import { Animation } from './animcontrol.js'

export default class Shorthands {
  meta = {
    name: 'shorthands'
  }

  get hooks() {
    const hooks = {
      setAnimParams: (ctx, next) => {
        this._normalize(ctx)
        next()
      }
    }
    hooks.setAnimParams.priority = 1
    return hooks
  }

  register(chart) {
    this.chart = chart
    this._channelNames = Object.keys(this.chart.config.channels)
  }

  _normalize(ctx) {
    if (!(ctx.target instanceof Animation)) {
      ctx.target = (Array.isArray(ctx.target) ? ctx.target : [{ ...ctx }])
        .map((keyframe) => (keyframe.target !== undefined ? keyframe : { target: keyframe }))
        .map((keyframe) => this._normalizeKeyframe(keyframe))
    }
    ctx.options = this._normalizeOptions(ctx.options)
  }

  _normalizeKeyframe(keyframe) {
    keyframe.target = this._normalizeTarget(keyframe.target)
    keyframe.options = this._normalizeOptions(keyframe.options)
    return keyframe
  }

  _normalizeTarget(target) {
    if (target && !(target instanceof CObject)) {
      if (!target.data && target.style === undefined && !target.config) {
        target = { config: target }
      }

      if (target.style === null) {
        target.style = { '': null }
      }
      target.style = target.style || {}

      target.config = this._normalizeConfig(target.config)
    }
    return target
  }

  _normalizeConfig(config) {
    if (config !== null && typeof config === 'object') {
      Object.keys(config).forEach((key) => {
        if (this._channelNames.includes(key)) {
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

  _normalizeChannels(channels) {
    Object.keys(channels).forEach((ch) => {
      if (typeof channels[ch] === 'string') {
        channels[ch] = [channels[ch]]
      }

      if (channels[ch] === null || Array.isArray(channels[ch])) {
        channels[ch] = { set: channels[ch] }
      }

      if (typeof channels[ch].attach === 'string') {
        channels[ch].attach = [channels[ch].attach]
      }

      if (typeof channels[ch].detach === 'string') {
        channels[ch].detach = [channels[ch].detach]
      }

      if (typeof channels[ch].set === 'string') {
        channels[ch].set = [channels[ch].set]
      }

      if (Array.isArray(channels[ch].set) && channels[ch].set.length === 0) {
        channels[ch].set = null
      }
    })
    return channels
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
