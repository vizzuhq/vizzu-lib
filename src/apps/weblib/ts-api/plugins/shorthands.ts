import * as Anim from '../types/anim.js'
import * as Config from '../types/config.js'
import * as Data from '../types/data.js'
import * as Styles from '../types/styles.js'
import { Plugin, PluginHooks, PrepareAnimationContext } from '../plugins.js'

import VizzuClass from '../vizzu.js'

import { Snapshot } from '../module/cchart.js'
import { CAnimation } from '../module/canimctrl.js'
import { AnimCompleting } from '../animcompleting.js'

export type AnySeriesList = Data.SeriesList | Data.SeriesDescriptor | null

export type LazyChannel = Config.Channel & {
  set?: AnySeriesList
  attach?: AnySeriesList
  detach?: AnySeriesList
}

export type AnyChannel = LazyChannel | AnySeriesList

export type LazyChannels = {
  [ChannelName in keyof Config.Channels]: AnyChannel
}

export type LazyChart = Omit<Config.Chart, 'channels'> & {
  channels?: LazyChannels
}

export type AnyChart = LazyChart & LazyChannels

export type LazyTarget = Anim.Target & {
  config?: AnyChart
}

export type AnyTarget = LazyTarget | AnyChart | Snapshot
export type AnyOptions = Anim.Options | Anim.Duration | null
export type AnyControlOptions = Anim.ControlOptions | Anim.Duration | null

export interface LazyKeyframe {
  target: AnyTarget
  options?: AnyOptions
}

export type AnyKeyframe = LazyKeyframe | AnyTarget
export type LazyKeyframes = AnyKeyframe[]
export type AnyAnimTarget = LazyKeyframes | AnyTarget | CAnimation
export type AnyAnimOptions = AnyControlOptions | AnyOptions

interface LazyPrepareAnimationContext {
  target: AnyAnimTarget
  options?: AnyAnimOptions
}

export declare class Vizzu extends VizzuClass {
  animate(target: AnyAnimTarget, options?: AnyAnimOptions): AnimCompleting
}

type ChannelName = keyof Config.Channels

export class Shorthands implements Plugin {
  private _chart?: VizzuClass
  private _channelNames?: string[]

  meta = {
    name: 'shorthands'
  }

  get hooks(): PluginHooks {
    const hooks = {
      prepareAnimation: Object.assign(
        (ctx: PrepareAnimationContext, next: () => void) => {
          this._normalize(ctx)
          next()
        },
        { priority: 1 }
      )
    }
    return hooks
  }

  register(chart: VizzuClass): void {
    this._chart = chart
    this._channelNames = Object.keys(this._chart.config.channels!)
  }

  private _normalize(ctx: LazyPrepareAnimationContext): void {
    if (!(ctx.target instanceof CAnimation)) {
      const target = this._normalizeKeyframes(ctx.target, ctx.options)
      if (ctx.options !== undefined && !this._isKeyframes(ctx.target)) delete ctx.options
      ctx.target = target
    }
    const options = this._normalizeOptions(ctx.options)
    if (options) ctx.options = options
  }

  private _normalizeKeyframes(
    target: LazyKeyframes | AnyTarget,
    options?: AnyAnimOptions
  ): Anim.Keyframes {
    const keyframes = this._isKeyframes(target) ? target : [this._toKeyframe(target, options)]

    return keyframes
      .map((keyframe) => (this._isKeyframe(keyframe) ? keyframe : { target: keyframe }))
      .map((keyframe) => this._normalizeKeyframe(keyframe))
  }

  private _toKeyframe(target: AnyTarget, options?: AnyOptions): LazyKeyframe {
    return Object.assign({ target }, options !== undefined ? { options } : {})
  }

  private _isKeyframes(value: AnyAnimTarget): value is LazyKeyframes {
    return Array.isArray(value)
  }

  private _isKeyframe(value: LazyKeyframe | AnyTarget): value is LazyKeyframe {
    return 'target' in value
  }

  private _normalizeKeyframe(keyframe: LazyKeyframe): Anim.Keyframe {
    const options = this._normalizeOptions(keyframe.options)
    return Object.assign(
      {
        target: this._normalizeTarget(keyframe.target)
      },
      options ? { options } : {}
    )
  }

  private _normalizeTarget(target: AnyTarget): Anim.Target | Snapshot {
    if (target && !(target instanceof Snapshot)) {
      if (this._isConfig(target)) {
        return { config: this._normalizeConfig(target) }
      }

      if (target.config !== undefined) {
        target.config = this._normalizeConfig(target.config)
      }

      if (target.style !== undefined) {
        target.style = this._normalizeStyle(target.style)
      }
    }
    return target
  }

  private _normalizeStyle(style: Styles.Chart | null | undefined): Styles.Chart {
    if (style === null) {
      return { '': null } as Styles.Chart
    }
    if (style === undefined) {
      return {}
    }
    return style
  }

  private _isTarget(value: LazyTarget | AnyChart): value is LazyTarget {
    return 'data' in value || 'style' in value || 'config' in value
  }

  private _isConfig(value: LazyTarget | AnyChart): value is LazyChart {
    return !this._isTarget(value)
  }

  private _normalizeConfig(config: AnyChart): Config.Chart {
    const { channels, ...rest } = this._normalizeConfigChannel(config)
    return {
      ...rest,
      ...(channels ? { channels: this._normalizeChannels(channels) } : {})
    }
  }

  private _normalizeConfigChannel(config: AnyChart): LazyChart {
    if (config !== null && typeof config === 'object') {
      config.channels = config.channels || ({} as LazyChannels)
      for (const prop in config) {
        const key = prop as keyof AnyChart
        if (this._isChannelName(key)) {
          const channel = config[key]
          if (channel !== undefined) {
            config.channels[key] = channel
          }
          delete config[key]
        }
      }
    }
    return config
  }

  private _isChannelName(value: string): value is ChannelName {
    return this._channelNames!.includes(value)
  }

  private _normalizeChannels(channels: LazyChannels): Config.Channels {
    const res = {} as Config.Channels
    for (const key in channels) {
      const ch = key as ChannelName
      const channel = channels[ch]
      if (channel !== undefined) res[ch] = this._normalizeChannel(channel)
    }
    return res
  }

  private _normalizeChannel(channel: AnyChannel): Config.Channel {
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

  private _normalizeOptions(options: AnyOptions | undefined): Anim.Options | undefined {
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
