/* eslint-disable @typescript-eslint/no-unused-vars, lines-between-class-members, no-use-before-define */

import { Data } from './data'
import { Config } from './config'
import { Styles } from './styles'

export namespace Anim {
  /** Duration can be set in seconds or milliseconds.
    In case no unit is set, it defaults to seconds. */
  type Duration = `${number}s` | `${number}ms` | number
  /** Seek position in the animation. */
  type Position = `${number}%` | Duration
  type Easing =
    | 'none'
    | 'linear'
    | 'step-start'
    | 'step-end'
    | 'ease'
    | 'ease-in'
    | 'ease-out'
    | 'ease-in-out'
    | `cubic-bezier(${number},${number},${number},${number})`
  /** Animation parameters for an animation group. */
  interface GroupOptions {
    /** Sets the easing used for the animation. */
    easing?: Easing
    /** The length of time an animation should take to complete. */
    duration?: Duration
    /** Waiting time interval before the animation starts. */
    delay?: Duration
  }
  /** Type of transition when the categorical series differ on the source and the target chart.
    - fade: the source chart fades out while the target chart fades in
    - drilldown: markers are splitted to be able to represent the target chart
    - aggregate: markers are aggregated then splitted differently to be
      able to represent the target chart.  */
  type RegroupStrategy = 'fade' | 'drilldown' | 'aggregate'
  /** If no animation settings are passed to Vizzu, it will use an automatic 
    setting depending on the actual configuration of the chart. This behavior can be
    overridden via the animation setting parameter.
    
    The animation between two states of the chart can require the transitioning
    of several different chart properties. These properties are grouped into 
    separately configurable animation groups.
    
    The parameters can also be set for the animation as a whole. These settings
    rescale the durations and delays of the animation groups to the 
    specified total delay and duration.  */
  interface Options extends GroupOptions {
    /** Animation group for style parameters. */
    style?: GroupOptions
    /** Title animation parameters. */
    title?: GroupOptions
    /** Subtitle animation parameters. */
    subtitle?: GroupOptions
    /** Caption animation parameters. */
    caption?: GroupOptions
    /** Legend animation parameters. */
    legend?: GroupOptions
    /** Animation group for new markers fading in 
    (due to filtering or added/removed data series).  */
    show?: GroupOptions
    /** Animation group for markers fading out 
    (due to filtering or added/removed data series).  */
    hide?: GroupOptions
    /** Marker color animation group. */
    color?: GroupOptions
    /** Coordinate system transformations animation group. */
    coordSystem?: GroupOptions
    /** Marker geometry morph animation group. */
    geometry?: GroupOptions
    /** Animation group for marker transitions in the direction of the y-axis. */
    y?: GroupOptions
    /** Animation group for marker transitions in the direction of the x-axis. */
    x?: GroupOptions
    /** Animation group for tooltip transitions. */
    tooltip?: GroupOptions
    /** Animation group for area/line rewiring transitions. */
    connection?: GroupOptions
    /** Selects the algorithm for transition in case of data grouped 
    differently on the source and target chart.  */
    regroupStrategy?: RegroupStrategy
  }
  interface ControlOptions {
    /** Determines if the animation should start automatically after the 
    animate() call.  */
    playState?: 'paused' | 'running'
    /** The starting position of the animation. */
    position?: number
    /** Play direction of the animation. */
    direction?: 'normal' | 'reverse'
    /** Playback speed of the animation. It is 1.0 by default.
    Negative values are considered 0. */
    speed?: number
  }
  /** Control object for animation. */
  interface Control {
    /** Seeks the animation to the position specified by time or progress 
    percentage. Seeking the animation to the end position will not trigger
    the {@link Vizzu.animate|animation promise} to resolve.  */
    seek(value: Position): void
    /** Pauses the controlled animation. */
    pause(): void
    /** Plays/resumes playing of the controlled animation. */
    play(): void
    /** Stops the current animation seeking it back to its start position. */
    stop(): void
    /** Changes the direction of the controlled animation. */
    reverse(): void
    /** Cancels the animation, will reject the animation promise. */
    cancel(): void
    /** Returns a reference to the actual animation for further reuse. */
    store(): CAnimation
    /** Playback speed of the animation. It is 1.0 by default.
    Negative values are considered 0. */
    speed: number
  }
  /** Promise resolves to the Vizzu object when the animation completed. */
  interface Completing extends Promise<Vizzu> {
    /** Promise resolves to the animation controller object when the animation starts. */
    activated?: Promise<Control>
  }
  /** Represents a state in the animation describing the data, the chart, and 
    the style parameters to be changed from the actual state.
    Passing null as style will reset every style parameter to default.  */
  interface Target {
    /** Data set. */
    data?: Data.Set
    /** Chart configuration changes. */
    config?: Config.Chart
    /** Style changes. */
    style?: Styles.Chart | null
  }
  /** All types, which can represent a single animation target chart state. */
  type LazyTarget = Target | Config.Chart | Snapshot
  /** All types, which can represent an animation option. */
  type LazyOptions = (Options | Duration) | null
  /** Object for describing a single animation target chart state and the 
    options of the animation to this chart state.   */
  interface Keyframe {
    target: Target | Snapshot
    options?: Options
  }
  /** Types, that can represent a Keyframe. */
  type LazyKeyframe = Keyframe | LazyTarget
  /** Sequence of keyframe descriptors */
  type Keyframes = Keyframe[]

  type AnimTarget = Keyframes | CAnimation
}
