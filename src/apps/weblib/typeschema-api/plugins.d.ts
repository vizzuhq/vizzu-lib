declare namespace Plugins {
  interface PluginMeta {
    name?: string
    version?: string
    depends?: string[]
  }

  interface SetAnimParamsContext {
    target: Anim.Keyframes | Anim.Animation | Anim.LazyTarget
    options: Anim.ControlOptions | (Anim.ControlOptions & Anim.LazyOptions)
  }

  interface AnimateRegisterContext {
    target: Anim.Keyframes | Anim.Animation | Anim.LazyTarget
    options: Anim.ControlOptions | (Anim.ControlOptions & Anim.LazyOptions)
    promise: Anim.Completing
  }

  interface PluginHooks {
    setAnimParams?: (ctx: SetAnimParamsContext, next: () => void) => void
    animateRegister?: (ctx: AnimateRegisterContext, next: () => void) => void
  }

  interface PluginListeners {
    [event: string]: (eventObj: any) => void
  }

  interface PluginApi {
    [key: string]: any
  }

  interface Plugin {
    meta?: PluginMeta
    hooks?: PluginHooks
    listeners?: PluginListeners
    api?: PluginApi
    register?: (ctx: Vizzu) => void
    unregister?: (ctx: Vizzu) => void
    enabled?: (enabled: boolean) => void
  }

  interface PluginApis {
    [name: string]: PluginApi
  }
}
