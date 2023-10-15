/* eslint-disable @typescript-eslint/no-unused-vars, lines-between-class-members, no-use-before-define */

import { Anim } from './anim'
import { Events } from './events'
import { Vizzu } from './vizzu'

/** Available hooks for plugins in Vizzu. */
export enum Hooks {
  /** Called when the animate() parameters gets set in the library to prepare 
      the animation. */
  setAnimParams = 'setAnimParams',
  /** Called when the animate() method called, and the lib shedules the call 
      to the animation queue. */
  animateRegister = 'animateRegister'
}

export namespace Plugins {
  /** Plugin metainfo. */
  interface PluginMeta {
    /** Name of the plugin. If not set, the class name will be used 
        with a lowercase first char. */
    name?: string
    /** Version of the plugin. Not used for now. */
    version?: string
    /** List of plugins this plugin depends on. 
        Dependent plugins should be registered beforehand. */
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

  /** Plugin hook implementation. Plugin hooks gets called by Vizzu in 
      priority order. Each hook should call the next hook in the chain,
      or the default implementation won't run. */
  type PluginHook<T> = {
    (ctx: T, next: Next): void
    priority?: number
  }

  /** Set of plugin hook implementations. */
  type PluginHooks<T extends Hooks = Hooks> = {
    [key in T]?: PluginHook<HookContexts[key]>
  }

  interface PluginListeners {
    [event: Events.Type]: Events.Handler<Events.EventMap[event]>
  }

  interface PluginApi {
    [apiMethod: string]: unknown
  }

  /** Vizzu plugin interface. */
  interface Plugin {
    /** Metainfo about the plugin. */
    meta?: PluginMeta
    /** Hooks the plugin implemenst. They work only if the plugin is enabled. */
    hooks?: PluginHooks
    /** Event listeners the plugin implements. They work only if the plugin is enabled. */
    listeners?: PluginListeners
    /** Any parameter or method the Plugin exposes for the user. */
    api?: PluginApi
    /** Register called when the plugin added to vizzu, receiving the Vizzu instance. */
    register?: (ctx: Parent) => void
    /** Unregister is called when detach() called on the Vizzu instance. */
    unregister?: (ctx: Parent) => void
    /** Method called by Vizzu indicating for the plugin that it got switched on/off.
        If switched off, its event listeners gets removed and its hooks won't be called. */
    enable?: (enabled: boolean) => void
  }

  interface PluginApis {
    [name: string]: PluginApi
  }
}
