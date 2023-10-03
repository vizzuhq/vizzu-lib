declare namespace Plugins {

interface PluginMeta {
  name: string;
  version?: string;
  depends?: string[];
}

interface VizzuContext {
  instance: Vizzu;
}

interface DataContext {
  data: Data.Set;
}

interface ConfigContext {
  config: Config.Chart;
}

interface StyleContext {
  style: Styles.Chart;
}

interface AnimOptionsContext {
  animOptions: Anim.LazyOptions;
}

interface AnimateRegisterContext {
  target: Anim.Keyframes | Anim.Animation | Anim.LazyTarget;
  options: Anim.ControlOptions | (Anim.ControlOptions & Anim.LazyOptions);
  promise: Anim.Completing;
}

interface PluginHooks {
  constructed?: (ctx: VizzuContext, next: () => void) => void;
  setData?: (ctx: DataContext, next: () => void) => void;
  setConfig?: (ctx: ConfigContext, next: () => void) => void;
  setStyle?: (ctx: StyleContext, next: () => void) => void;
  setAnimOptions?: (ctx: AnimOptionsContext, next: () => void) => void;
  animateRegister?: (ctx: AnimateRegisterContext, next: () => void) => void;
}

interface PluginListeners {
  [event: string]: (eventObj) => void;
}

interface PluginApi {
  [apiMethod: string]: (...args: any[]) => any;
}

interface Plugin {
  meta: PluginMeta;
  hooks?: PluginHooks;
  listeners?: PluginListeners;
  api?: PluginApi;
  register?: (ctx) => void;
  unregister?: (ctx) => void;
  enabled?: (boolean) => void;
}

interface PluginApis {
  [name: string]: PluginApi;
}

}