/* eslint-disable @typescript-eslint/no-unused-vars, lines-between-class-members, no-use-before-define */

import { Anim } from './anim'
import { Events } from './events'
import { Vizzu } from './vizzu'

export enum Hooks {
  setAnimParams = 'setAnimParams',
  animateRegister = 'animateRegister'
}

export namespace Plugins {
  interface PluginMeta {
    name?: string
    version?: string
    depends?: string[]
  }

  interface SetAnimParamsContext {
    target: Anim.AnimTarget
    options?: Anim.ControlOptions | (Anim.ControlOptions & Anim.Options)
  }

  interface AnimateRegisterContext {
    target: Anim.AnimTarget
    options?: Anim.ControlOptions | (Anim.ControlOptions & Anim.Options)
    promise: Anim.Completing
  }

  interface HookContexts {
    [Hooks.setAnimParams]: SetAnimParamsContext
    [Hooks.animateRegister]: AnimateRegisterContext
  }

  type Next = () => void

  type PluginHook<T> = {
    (ctx: T, next: Next): void
    priority?: number
  }

  type PluginHooks<T extends Hooks = Hooks> = {
    [key in T]?: PluginHook<HookContexts[key]>
  }

  interface PluginListeners {
    [event: Events.Type]: Events.Handler<Events.EventMap[event]>
  }

  interface PluginApi {
    [apiMethod: string]: unknown
  }

  interface Plugin {
    meta?: PluginMeta
    hooks?: PluginHooks
    listeners?: PluginListeners
    api?: PluginApi
    register?: (ctx: Parent) => void
    unregister?: (ctx: Parent) => void
    enable?: (enabled: boolean) => void
  }

  interface PluginApis {
    [name: string]: PluginApi
  }
}
