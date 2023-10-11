/// <reference types="../types/vizzu.d.ts" />

import Vizzu from "../vizzu"

import { Snapshot, CAnimation } from '../module/cchart.js'

type ChannelName = keyof Config.Channels

export class Shorthands implements Plugins.Plugin {
  chart?: Vizzu
  _channelNames?: string[]

  meta = {
    name: 'shorthands'
  }

  get hooks() {
    const hooks = {
      setAnimParams: Object.assign((ctx: Plugins.SetAnimParamsContext, next: () => void) => {
        this._normalize(ctx)
        next()
      }, { priority: 1 })
    }
    return hooks
  }

  register(chart: Vizzu) {
    this.chart = chart
    this._channelNames = Object.keys(chart.config.channels!)
  }

  _normalize(ctx: Plugins.SetAnimParamsContext) {
    if (!(ctx.target instanceof CAnimation)) {
      ctx.target = (Array.isArray(ctx.target) ? ctx.target : [{ ...ctx }])
        .map((keyframe) => (keyframe.target !== undefined ? keyframe : { target: keyframe }))
        .map((keyframe) => this._normalizeKeyframe(keyframe))
    }
    ctx.options = this._normalizeOptions(ctx.options)
  }

  _normalizeKeyframe(keyframe: Anim.Keyframe) {
    keyframe.target = this._normalizeTarget(keyframe.target)
    keyframe.options = this._normalizeOptions(keyframe.options)
    return keyframe
  }

  _normalizeTarget(target: Anim.Target | Config.Chart | Snapshot): Anim.Target | Snapshot {
    if (target && !(target instanceof Snapshot)) {
      if (this._isConfig(target)) {
        target = { config: target }
      }

      if (target.style === null) {
        target.style = { '': null } as Styles.Chart
      }
      target.style = target.style || ({} as Styles.Chart)

      target.config = this._normalizeConfig(target.config)
    }
    return target
  }

  _isConfig(value: Anim.Target | Config.Chart): value is Config.Chart {
    return !('data' in value || 'style' in value || 'config' in value)
  }

  _normalizeConfig(config: Config.Chart | undefined): Config.Chart | undefined {
    if (config !== null && typeof config === 'object') {
      Object.keys(config).forEach((key) => {
        if (this._isChannelName(key)) {
          config.channels = config.channels || ({} as Config.Channels)
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

  _isChannelName(value: string): value is ChannelName {
    return this._channelNames!.includes(value)
  }

  _normalizeChannels(channels: Config.Channels) {
    Object.keys(channels).forEach((key) => {
      const ch = key as ChannelName
      channels[ch] = this._normalizeChannel(channels[ch])
    })
    return channels
  }


  _normalizeChannel(channel: Config.Channel | Data.SeriesList | null | undefined): Config.Channel | undefined {
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

  _normalizeOptions(options: Anim.ControlOptions & Anim.LazyOptions | undefined) {
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
