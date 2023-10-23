/* eslint-disable @typescript-eslint/no-unused-vars, lines-between-class-members, no-use-before-define */

import { Plugins } from './plugins'

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
