/* eslint-disable @typescript-eslint/no-unused-vars, lines-between-class-members, no-use-before-define */

import { Lib } from './lib'
import { Data } from './data'
import { Config } from './config'
import { Styles } from './styles'
import { Anim } from './anim'
import { Geom } from './geom'
import { Events } from './events'
import { Presets } from './presets'
import { Plugins } from './plugins'

export { Lib }
export { Data }
export { Config }
export { Styles }
export { Anim }
export { Geom }
export { Events }
export { Presets }
export { Plugins }

/** Stored Animation object. */
declare class CAnimation {}
/** Stored Chart object. */
declare class Snapshot {}
/** List of base and additional features:
    - logging: enables logging of the library to the console 
      (switched off by default).
    - rendering: enables rendering of the library to the canvas
      (enabled by default). 
    - tooltip: tooltips on the chart appearing on markers on mouse over. 
      Since the tooltip uses the animation interface, calling animate() while
      the tooltip is enabled can cause unwanted behaviour.
    - cssProperties: enables setting the styles through --vizzu-... css properties.
    - shorthands: enables shorthand properties for chart config and style.
    - pivotData: enables setting the data in pivot table or data cube format.
    - pointerEvents: enables pointer events on the chart.
  */
type Feature =
  | 'logging'
  | 'tooltip'
  | 'rendering'
  | 'cssProperties'
  | 'shorthands'
  | 'pivotData'
  | 'pointerEvents'

export interface VizzuOptions {
  container: HTMLElement
  features?: Plugins.Plugin[]
}

export type FeatureFunction = (
  feature: Feature | Plugins.Plugin,
  enabled?: boolean
) => Plugins.PluginApi
export interface Features extends Record<string, Plugins.PluginApi>, FeatureFunction {}

/** Class representing a single chart in Vizzu. */
export default class Vizzu {
  /** Creates a new chart and connects it to the div or canvas HTML 
    element specified by its ID or DOM object. The new chart is empty by 
    default, but can be set to an initial state in the second optional 
    parameter. */
  constructor(
    container: string | HTMLElement | VizzuOptions,
    initState?: Anim.Target | Config.Chart
  )
  /** Promise representing the initialization will resolve when 
    initialization is finished. Any API call will potentially cause 
    an error before this promise is resolved.  */
  initializing: Promise<Vizzu>
  /** Installs the provided event handler to the event specified by name. */
  on<T extends Events.Type>(eventName: T, handler: Events.Handler<Events.EventMap[T]>): void
  /** Uninstalls the provided event handler from the event specified by name. */
  off<T extends Events.Type>(eventName: T, handler: Events.Handler<Events.EventMap[T]>): void
  /** Initiates the animation either to the new chart state passed as the first 
    argument, or through a sequence of keyframe charts passed as the first
    argument. If there is a currently running animation, all subsequent 
    calls will schedule the corresponding animation after the end of the 
    previous one.
    The new chart state or keyframe can be a full state specifier object with 
    data, config and style, or a single chart config object.
    It accepts also a chart snapshot acquired from a previous state using 
    the store() method of this class or a whole previous animation acquired
    using the store() method of the Anim.Control object, which can be queried
    from the promise returned by the animate() method.
    The optional second parameter specifies the animation control options 
    and also all the other animation options in case of only a single chart
    state passed as the first argument. 
    This second option can be a scalar value, setting the overall 
    animation duration. Passing explicit null as second parameter will
    result in no animation.
    The method returns a promise, which will resolve when the animation is
    finished. Since there can be multiple animations in the queue, the result
    promise provides a nested promise member {@link Anim.Completing.activated|activated}, 
    which resolves when the requested animation gets active.  */
  animate(
    animTarget: Anim.Keyframes | CAnimation,
    animOptions?: Anim.ControlOptions | (Anim.ControlOptions & Anim.LazyOptions)
  ): Anim.Completing
  /** Returns a reference to the actual chart state for further reuse. 
    This reference includes the chart config, style parameters and the
    data filter but does not include the actual data and the animation options. */
  store(): Snapshot
  /** Returns controls for the ongoing animation, if any.
    @deprecated since version 0.4.0  */
  get animation(): Anim.Control
  /** Returns the version number of the library. */
  version(): string
  /** Returns the underlying canvas element. */
  getCanvasElement(): HTMLCanvasElement
  /** Returns a converter function. */
  getConverter(
    target: `plot${string}`,
    from: Geom.CoordinateType,
    to: Geom.CoordinateType
  ): Geom.Converter
  /** Re-renders the chart. */
  forceUpdate(): void
  /** Property for read-only access to style object without default values. */
  get style(): Readonly<Styles.Chart>
  /** Property for read-only access to the style object after setting defaults. */
  getComputedStyle(): Readonly<Styles.Chart>
  /** Property for read-only access to chart parameter object. */
  get config(): Readonly<Config.Chart>
  /** Property for read-only access to data metainfo object. */
  get data(): Readonly<Data.Metainfo>
  /** If called as a function:
      (name, enabled): it enables/disables built-in features and registered plugins. 
      (plugin, enabled?): registers the given plugin.
      Otherwise gives access to the interfaces of the registered plugins, where
      every plugin acceccible as a property with the plugin name.
  */
  get feature(): Features
  /** Removes the reference of the chart from every place it attached itself,
    this method must be called in order to get the chart properly garbage 
    collected.  */
  detach(): void
  /** Returns the chart preset collection. */
  static get presets(): Presets.Presets
  /** Setter method for Library options. */
  static options(options: Lib.Options): void
  /** Initializes the library, if not called, first constructor call will do that. */
  static initialize(): Promise<void>
}
