/* eslint-disable @typescript-eslint/no-unused-vars, lines-between-class-members, no-use-before-define */

import { Data } from './data'

export namespace Config {
  /** Units: 
    - no unit: the same unit as in the data;
    - %: percentage relative to the min/max of the data; 
    - min,max: offset from min/max of the data;
    - auto: automatic range based on chart config; */
  type ChannelExtrema = number | `${number}%` | `${number}min` | `${number}max` | 'auto'
  /** Channel range specifies how to scale the represented data. */
  interface ChannelRange {
    min?: ChannelExtrema | null
    max?: ChannelExtrema | null
  }
  type AutoBool = boolean | 'auto'
  /** Channels are the main building blocks of the chart. Each channel describes
    a particular aspect of the markers (vertical & horizontal position, color, etc.)
    and connects them to the underlying data. A single measure and an ordered list of
    dimensions can be on each channel. The dimensions will recursively slice the
    measure on the channel. The channels are represented on the chart as an 
    axis or legend. */
  interface Channel {
    /** This title is shown on the axis or legend corresponding to the channel.
    If 'auto', the title will be the name of the measure attached to
    that channel. */
    title?: (string | 'auto') | null
    /** List of {@link SeriesDescriptor|data series descriptors} on the channel. */
    set?: Data.SeriesList | null
    /** List of {@link SeriesDescriptor|data series descriptors} to be added to the 
    channel beside the ones already added. */
    attach?: Data.SeriesList
    /** List of {@link SeriesDescriptor|data series descriptors} to be removed from the 
    channel. */
    detach?: Data.SeriesList
    /** Specifies the range that determines how the represented data scales 
    on the channel. */
    range?: ChannelRange
    /** Only one dimension can be shown on an axis or legend by
    name. This index specifies which attached series should be used. */
    labelLevel?: number
    /** Enables the axis line on axis channels. */
    axis?: AutoBool
    /** Enables the axis labels on axis channels. */
    labels?: AutoBool
    /** Enables the axis ticks on axis channels showing measure data. */
    ticks?: AutoBool
    /** Enables the interlacing on axis channels showing measure data. */
    interlacing?: AutoBool
    /** Enables the grid lines on axis channels showing dimension data. */
    guides?: AutoBool
    /** Enables the guide lines on axis channels showing measure data
    for all marker positions. */
    markerGuides?: AutoBool
    /** Base interval of the interlacing on the axis. */
    step?: number | 'auto'
  }
  /** Channel configuration. 
    A data series' name or a list of the data series' names can be used as a 
    short-hand - instead of the {@link Config.Channel|channel object} - to set data series 
    for the channel. Setting a channel to null will remove all data series from it. */
  interface Channels {
    /** Parameters for the X-axis, determining the position of the markers on the 
    x-axis - or their angle when using polar coordinates. 
    Note: leaving x and y channels empty will result in a 
    chart "without coordinates" like a Treemap or a Bubble Chart. */
    x?: (Channel | Data.SeriesList) | null
    /** Parameters for the Y-axis, determining the position of the markers on the 
    y-axis - or their radius when using polar coordinates) . */
    y?: (Channel | Data.SeriesList) | null
    /** Parameters for the markers' base color. The markers' actual color can 
    also be affected by the lightness channel. */
    color?: (Channel | Data.SeriesList) | null
    /** Parameters for markers' lightness. */
    lightness?: (Channel | Data.SeriesList) | null
    /** Parameters for the markers' size. Effective only for circle and line
    geometry affecting the circle area or the line width respectively. */
    size?: (Channel | Data.SeriesList) | null
    /** Parameters for the content of the labels that appear on the markers. */
    label?: (Channel | Data.SeriesList) | null
    /** Splits the markers as all the other channels, but have no 
    effect on the markers' appearance. Thus, it only works with dimensions. */
    noop?: (Channel | Data.SeriesList) | null
  }
  /** The config contains all of the parameters needed to render a particular 
    static chart or a state of an animated chart. */
  interface Chart extends Channels {
    /** List of the chart's channels' configuration. The chart object also 
    extends the channels object as a configuration shorthand. */
    channels?: Channels
    /** This is the title shown on the top of the chart.
    If set to null, the title will not be shown and will not take up any
    space in the chart layout. */
    title?: string | null
    /** This is the subtitle shown on the top of the chart under the title.
    If set to null, the subtitle will not be shown and will not take up any
    space in the chart layout. */
    subtitle?: string | null
    /** This is the caption shown on the bottom of the chart.
    If set to null, the caption will not be shown and will not take up any
    space in the chart layout. */
    caption?: string | null
    /** Specifies which channel should be shown on the legend.
    If set to null, the legend will not be shown and will not take up any
    space in the chart layout.
    If set to auto, the internal logic will select the most suitable channel
    for the legend. */
    legend?: 'color' | 'lightness' | 'size' | 'auto' | null
    /** Sets the coordinate system for the chart. Switch to the 'polar' 
    coordinate system to create a chart from the pie/radial chart family. */
    coordSystem?: 'cartesian' | 'polar'
    /** Rotates the plot area by the specified angle in degree. 
    Note: this is an experimental, not tested feature. */
    rotate?: number
    /** Sets the geometric elements used for the markers to represent the data. */
    geometry?: 'rectangle' | 'circle' | 'area' | 'line'
    /** If both axes have measures on them, this parameter sets the 
    orientation of the chart, meaning to which axis the graphical elements 
    are oriented to. */
    orientation?: 'auto' | 'horizontal' | 'vertical'
    /** - 'none': markers are sorted in the order as the corresponding data 
              appear in the data set.
    - 'byValue': markers will be sorted by the corresponding measure (if present)
    in decreasing order. */
    sort?: 'none' | 'byValue'
    /** Reverts the order of the markers if set. */
    reverse?: boolean
    /** Sets the alignment of the markers with relation to the x- or the y-axis depending
    on where the measure is. In case both axes have measures on them, this is determined 
    by the {@link Config.Chart.orientation|orientation} of the chart. */
    align?: 'none' | 'min' | 'center' | 'max' | 'stretch'
    /** If set to true, markers will be split by the dimension(s) along the axis.
    This works if you have at least one dimension and a measure on the same axis.In case 
    both axes have measures and dimension(s) on them, this is determined by the 
    {@link Config.Chart.orientation|orientation} of the chart. */
    split?: boolean
  }
}
