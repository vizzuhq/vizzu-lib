declare namespace Lib {
  /** Options for the library. */
  interface Options {
    /** The URL for the webassembly binary (cvizzu.wasm). */
    wasmUrl?: string;
  }
}
declare namespace Data {
  /** Name of the data series. */
  type SeriesName = string
  /** Type of the data series:
    - 'dimension' - categorical data containing strings 
      (dates should also be added as strings); 
    - 'measure' - continuous data containing numbers. */
  type SeriesType = 'dimension' | 'measure'
  /** Additional info about a data series besides the contained data. */
  interface SeriesMetaInfo {
    /** Name of the data series. It will be the unique id of the series to 
    reference it in various parts of the API, mainly in {@link Config.Channel} and
    {@link Data.Record}. This name will also be used by default for Axis and 
    Legend title. */
    name: SeriesName
    /** Type of the data series:
    If not set, the library will attempt to determine the type based on 
    the type of the first value. Number type will result in measure, 
    string type will result in dimension. */
    type?: 'dimension' | 'measure'
    /** Unit of the data series. */
    unit?: string
  }
  interface AbstractSeriesInfo extends SeriesMetaInfo {
    /** Count of values in the series. */
    length: number
  }
  /** Meta data about dimension data series */
  interface DimensionSeriesInfo extends AbstractSeriesInfo {
    /** Distinct values in the series */
    categories: string[]
  }
  interface Range {
    /** Minimal value in the series */
    min: number
    /** Maximal value in the series */
    max: number
  }
  /** Meta data about measure data series */
  interface MeasureSeriesInfo extends AbstractSeriesInfo {
    range: Range
  }
  type SeriesInfo = DimensionSeriesInfo | MeasureSeriesInfo
  /** Meta data about the data set */
  interface Metainfo {
    series: SeriesInfo[]
  }
  /** Represents a categorical or data value */
  type Value = string | number
  /** List of data values in a series. */
  type Values = string[] | number[]
  /** Defines a data series of the data set, and contains a particular variable's
    values in the data set and meta info about the variable. */
  interface Series extends SeriesMetaInfo {
    /** The array that contains the values of the data series. The value types 
    should match {@link Data.SeriesMetaInfo.type}. If the data series
    is shorter than the longest data series defined, it will be internally 
    extended with empty values. */
    values?: Values
  }
  /** A record of the data set, containing one value of each data series 
    corresponding to the same index. */
  interface Record {
    /** Properties are provided for each data series, providing access to the value within
    the record referenced by its {@link Data.Series.name|name}. */
    [seriesName: SeriesName]: Value
  }
  type FilterCallback = (record: Record) => boolean
  interface Filter {
    /** A filter callback is called on each record of the dataset on chart
    generation. If the callback returns false, the record will not be shown on the chart. */
    filter?: FilterCallback | null
  }
  /** Data table specified by series. */
  interface TableBySeries extends Filter {
    /** The series that make up the the data set. */
    series: Series[]
  }
  /** Values of a data record in the order of the series in the dataset. */
  type ValueArray = Value[]
  /** Data table specified by records. */
  interface TableByRecords extends Filter {
    /** The information about the data series in the records of the data set. 
    Note: not needed if it was previously specified. */
    series?: SeriesMetaInfo[]
    /** The array of data records that make up the data set. */
    records: (ValueArray | Record)[]
  }
  type CubeRow = Values | CubeRow[]
  /** Defines a data series of the data cube, and contains a particular variable's
    values in the data cube and meta info about that variable. */
  interface CubeData extends SeriesMetaInfo {
    /** A nested array that contains the values of the data series. Nesting 
    level should match the number of {@link Data.Cube.dimensions}. */
    values: CubeRow
  }
  /** N dimensional data cude */
  interface Cube extends Filter {
    /** The list of the dimensions of the data cube. */
    dimensions: Series[]
    /** The list of measures of the data cube. */
    measures: CubeData[]
  }
  /** Data set is a collection of related {@link Data.Series|data series}. 
    Each chart works on a single data set. */
  type Set = TableBySeries & TableByRecords
  /** Types of the different available data series aggregators:
    
    - sum: sum of the values (default)
    - count: count of the values
    - min: minima of the values
    - max: maxima of the values
    - mean: average/mean of the values
    - distinct: number of different values */
  type AggregatorType = 'sum' | 'count' | 'min' | 'max' | 'mean' | 'distinct'
  /** The name of a series either alone or combined with an aggregator function. */
  type SeriesDescriptor = SeriesName | `${AggregatorType}(${SeriesName})`
  type SeriesList = SeriesDescriptor[] | SeriesDescriptor
}
declare namespace Config {
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
    orientation?: 'horizontal' | 'vertical'
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
declare namespace Styles {
  /** Length can be set in pixels or in percentage of the element or the element's
    font size. Pixel is the default unit.  */
  type Length = `${number}px` | `${number}%` | `${number}em` | number
  /** Angle can be set in radians, degrees, gradians and turns. 
    Radians is the default unit.  */
  type Angle = `${number}rad` | `${number}grad` | `${number}deg` | `${number}turn` | number
  /** The following CSS color formats are available: 
    rgb(), rgba(), #RRGGBB, #RRGGBBAA, #RGB. */
  type Color =
    | `#${string}`
    | `rgb(${number},${number},${number})`
    | `rgba(${number},${number},${number},${number})`
  /** Number scale for human readable big number formats.
    There are built in formats:
    - SI Symbols: k, M, G, ...
    - Short scale with US abbreviations: K, M, B, T
    - Short scale with UK abbreviations: k, m, bn, tn
    Can be set to custom format with a comma separated list of strings 
    e.g: 'thousand,million,billion,trillion' */
  type NumberScale =
    | 'SISymbol'
    | 'shortScaleSymbolUS'
    | 'shortScaleSymbolUK'
    | `${string},${string}`
    | `${string},${string},${string}`
    | `${string},${string},${string},${string}`
    | string
  interface Padding {
    /** Top padding of the element. */
    paddingTop?: Length | null
    /** Right padding of the element. */
    paddingRight?: Length | null
    /** Bottom padding of the element. */
    paddingBottom?: Length | null
    /** Left padding of the element. */
    paddingLeft?: Length | null
  }
  interface Font {
    /** The family of the font. If not set, it inherits the root style font family. */
    fontFamily?: string | null
    /** The style of the font. */
    fontStyle?: 'normal' | 'italic' | 'oblique' | null
    /** The weight of the font, numbers use the same scale as CSS. */
    fontWeight?: ('normal' | 'bold' | number) | null
    /** The size of the font. Percentage values are relative to the root style font size. */
    fontSize?: Length | null
  }
  interface Box {
    /** The background color of the element. */
    backgroundColor?: Color | null
    /** The border color of the element. */
    borderColor?: Color | null
    /** The border width of the element. */
    borderWidth?: number | null
  }
  interface Text {
    /** The color of the displayed text. */
    color?: Color | null
    /** The alignment of the displayed text. */
    textAlign?: 'center' | 'left' | 'right' | null
    /** The background color of the displayed text. */
    backgroundColor?: Color | null
    /** The format of the number. Only applicable for texts showing numerical
    data such as marker and axis labels. 'grouped' uses thousand separators
    (e.g. 1 234 567), 'prefixed' uses scientific notation (e.g. 1.234k).  */
    numberFormat?: 'none' | 'grouped' | 'prefixed' | null
    /** The maximum number of digits in fraction part if the text contains a number. */
    maxFractionDigits?: number | null
    /** Number scale used for prefixed number format. */
    numberScale?: NumberScale | null
  }
  /** The following CSS like filters can be used to alter the color: 
    
    - color: overrides the color.
    - lightness: lightens or darkens the color; 0 means the original color, -1 
                 means black, 1 means white.
    - grayscale: desaturates the color. 0 means the original color, 1 means fully
                 desaturated.
    - none: no change. */
  type ColorTransform =
    | `color(${Color})`
    | `lightness(${number})`
    | `grayscale(${number})`
    | `opacity(${number})`
    | 'none'
  interface OrientedLabel extends Label {
    /** Orientation of the label relatively to the axis or marker it is attached to. */
    orientation?: 'normal' | 'tangential' | 'horizontal' | 'vertical' | null
    /** Additional rotation of the label. */
    angle?: Angle | null
  }
  interface MarkerLabel extends OrientedLabel {
    /** The label position relatively to the marker. */
    position?: 'center' | 'top' | 'left' | 'bottom' | 'right' | null
    /** Defines the transformation used for calculating the label color
    from the marker color.  */
    filter?: ColorTransform | null
    /** Sets the order of values on the label if both a measure and a dimension are
    present.  */
    format?: 'measureFirst' | 'dimensionsFirst' | null
  }
  interface Guides {
    /** The color of the guides. */
    color?: Color | null
    /** Line width of the guide in pixel. */
    lineWidth?: number | null
  }
  interface Tooltip extends Font, Box {
    /** The layout of the tooltip text */
    layout?: 'singleLine' | 'multiLine'
    /** The foreground color of the tooltip text */
    color?: Color
    /** Color of the drop shadow */
    shadowColor?: Color
    /** Corner radius for the info bubble */
    borderRadius?: number
    /** Drop shadow distance from the info bubble */
    dropShadow?: number
    /** Data point marker radius */
    radius?: number
    /** Base size of the info bubble arrow */
    arrowSize?: number
    /** Distance between the data point and the info bubble */
    distance?: number
    /** Specifies the name of the data series dispalyed
    at the first position on the tooltip  */
    seriesName?: string | null
  }
  interface Logo extends Padding {
    /** Width of the Vizzu logo */
    width?: Length | null
    /** Color transformation applied on the colored Vizzu Logo */
    filter?: ColorTransform | null
  }
  interface DataPoint {
    /** Color gradient used for the measure on the color channel. */
    colorGradient?: ColorGradient | null
    /** Color palette used for the dimension on the color channel. */
    colorPalette?: ColorPalette | null
    /** Lightness value associated with the minimum value of the lightness 
    channel's range.  */
    minLightness?: number | null
    /** Lightness value associated with the maximum value of the lightness 
    channel's range.  */
    maxLightness?: number | null
    /** Minimum of line width specified as proportion of plot area size. 
    e.g.: 0.01 means 1% of the width of the plot area.  */
    lineMinWidth?: number | null
    /** Maximum line width specified as proportion of plot area size.
    e.g.: 0.01 means 1% of the width of the plot area.  */
    lineMaxWidth?: number | null
    /** Minimum circle radius specified as proportion of plot area size.
    e.g.: 0.01 means 1% of the width of the plot area.  */
    circleMinRadius?: number | null
    /** Maximum circle radius specified as proportion of plot area size. 
    e.g.: 0.01 means 1% of the width of the plot area.  */
    circleMaxRadius?: number | null
    /** Spacing between bars/columns. The value specifies the size of the 
    spacing as a factor of the marker size. 
    e.g.: 0.1 means 10% of marker height/width depending on the chart's orientation.  */
    rectangleSpacing?: number | null
  }
  interface Marker extends DataPoint {
    /** Width of the marker border in pixels. */
    borderWidth?: number | null
    /** Opacity of the marker border. */
    borderOpacity?: number | null
    borderOpacityMode?: 'straight' | 'premultiplied' | null
    /** Opacity of the marker fill color. */
    fillOpacity?: number | null
    /** Style settings for guide lines drawn for the markers. */
    guides?: Guides | null
    /** Style settings for the marker labels. */
    label?: MarkerLabel | null
  }
  /** Style settings for the values shown on the axis to display the scale 
    being used or the categories along the axis.
    Note: textAlign has no effect on the Axis label.  */
  interface AxisLabel extends OrientedLabel {
    /** Label position relatively to the plot. */
    position?: 'axis' | 'max-edge' | 'min-edge' | null
    /** Label alignment relatively to the position on the plot. */
    side?: 'positive' | 'negative' | null
  }
  /** Style settings of the {@link Config.Channel.title|Axis title} */
  interface AxisTitle extends Label {
    /** Title position relatively to the plot. */
    position?: 'axis' | 'min-edge' | 'max-edge' | null
    /** Title alignment relatively to the position on the plot. */
    side?: 'positive' | 'upon' | 'negative' | null
    /** Title position on the axis or edge. */
    vposition?: 'begin' | 'middle' | 'end' | null
    /** Title alignment on the axis or edge. */
    vside?: 'positive' | 'upon' | 'negative' | null
    /** The orientation of the title. */
    orientation?: 'horizontal' | 'vertical' | null
  }
  interface Ticks {
    /** Color of the ticks on the axis. */
    color?: Color | null
    /** Line width of the ticks on the axis. */
    lineWidth?: number | null
    /** Length of the ticks on the axis. */
    length?: Length | null
    /** Position of the ticks on the axis relatively to the axis line. */
    position?: 'outside' | 'inside' | 'center' | null
  }
  interface Interlacing {
    /** Color of the interlacing pattern. */
    color?: Color | null
  }
  interface Axis {
    /** Color of the axis line. */
    color?: Color | null
    /** Style parameters of the axis title. */
    title?: AxisTitle | null
    /** Style parameters of the axis labels. */
    label?: AxisLabel | null
    ticks?: Ticks | null
    guides?: Guides | null
    interlacing?: Interlacing | null
  }
  interface Plot extends Padding, Box {
    /** Style settings for the markers. */
    marker?: Marker | null
    /** Style settings for the x-axis - or the angle when using polar coordinates. */
    xAxis?: Axis | null
    /** Style settings for the y-axis - or the radius when using polar coordinates. */
    yAxis?: Axis | null
    /** Color of the plot area. */
    areaColor?: Color | null
    /** Controls drawing outside of the plot area. If hidden, clipping will be
    set for the boundary of the coordinate system.  */
    overflow?: 'hidden' | 'visible' | null
  }
  interface LegendMarker {
    /** Shape of the legend marker. */
    type?: 'circle' | 'square' | null
    /** Size of the legend marker (diameter or side length). */
    size?: Length | null
  }
  interface Legend extends Padding, Box {
    /** Width of the legend's boundary box. */
    width?: Length | null
    /** Limit for the width of the boundary box. */
    maxWidth?: Length | null
    /** Style settings for the legend's title. */
    title?: Label | null
    /** Style settings for the labels on the legend. */
    label?: Label | null
    marker?: LegendMarker | null
  }
  /** Color and position pairs separated by spaces, 
    where position is a number between 0 and 1. */
  type ColorStop = `${Color} ${number}`
  /** Color gradient is specified by a comma separated list of ColorStops. 
    This is used when a measure is on the color channel.  */
  type ColorGradient =
    | ColorStop
    | `${ColorStop},${ColorStop}`
    | `${ColorStop},${ColorStop},${ColorStop}`
    | `${ColorStop},${ColorStop},${ColorStop},${ColorStop}`
    | `${ColorStop},${ColorStop},${ColorStop},${ColorStop},${ColorStop}`
    | string
  /** Color palette is a list of colors separated by spaces. 
    This is used when only dimensions are on the color channel */
  type ColorPalette =
    | Color
    | `${Color} ${Color}`
    | `${Color} ${Color} ${Color}`
    | `${Color} ${Color} ${Color} ${Color}`
    | `${Color} ${Color} ${Color} ${Color} ${Color}`
    | string
  type Label = Padding & Font & Text
  interface Chart extends Padding, Box, Font {
    /** Style settings for the plot area. */
    plot?: Plot | null
    /** Style settings for the legend. */
    legend?: Legend | null
    /** Style settings for the main chart title. */
    title?: Label | null
    /** Style settings for the tooltip. */
    tooltip?: Tooltip | null
    /** Style settings of the Vizzu logo. */
    logo?: Logo | null
  }
}
declare namespace Anim {
  /** Duration can be set in seconds or milliseconds.
    In case no unit is set, it defaults to seconds. */
  type Duration = `${number}s` | `${number}ms` | number
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
  }
  /** Promise resolves to the Vizzu object when the animation completed. */
  interface Completing extends Promise<Vizzu> {
    /** Promise resolves to the animation controller object when the animation starts. */
    activated: Promise<Control>
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

  type AnimTarget = Anim.Keyframes | CAnimation
}
declare namespace Geom {
  /** Object representing a 2D point. */
  interface Point {
    /** X coordinate of the point. */
    x: number
    /** Y coordinate of the point. */
    y: number
  }
  /** Object representing a rectangle. */
  interface Rect {
    /** Position of the rectangle's top-left corner. */
    pos: Point
    /** Size of the rectangle. */
    size: Point
  }
  /** Object representing a line section. */
  interface Line {
    /** Starting point of the line. */
    begin: Point
    /** End point of the line. */
    end: Point
  }
  /** Object representing an affine transformation 2x3 matrix. */
  type AffineTransform = number[][]
  /** Rectangle transformed by an affine transformation. */
  interface TransformedRect {
    /** Transform transforms the top left corner to the origo. */
    transform: AffineTransform
    /** Size of the rectangle. */
    size: Point
  }
}
declare namespace Event {
  type Type =
    | 'click'
    | 'pointeron'
    | 'pointermove'
    | 'pointerdown'
    | 'pointerup'
    | 'wheel'
    | 'update'
    | 'draw-begin'
    | 'draw-complete'
    | 'background-draw'
    | 'title-draw'
    | 'logo-draw'
    | 'legend-background-draw'
    | 'legend-title-draw'
    | 'legend-label-draw'
    | 'legend-marker-draw'
    | 'legend-bar-draw'
    | 'plot-background-draw'
    | 'plot-area-draw'
    | 'plot-marker-draw'
    | 'plot-marker-label-draw'
    | 'plot-marker-guide-draw'
    | 'plot-axis-draw'
    | 'plot-axis-title-draw'
    | 'plot-axis-label-draw'
    | 'plot-axis-tick-draw'
    | 'plot-axis-guide-draw'
    | 'plot-axis-interlacing-draw'
    | 'animation-begin'
    | 'animation-complete'
  /** Abstract Element interface for chart elements. */
  interface Element {
    /** Name of the chart element type. */
    tagName: string
    /** Parent element of the chart element. */
    parent?: Element
  }
  /** Element with a string value. */
  interface TextElement extends Element {
    value: string
  }
  /** Element with an orientation value. */
  interface OrientedElement extends Element {
    id: 'x' | 'y'
  }
  /** Root element of the chart. */
  interface Root extends Element {
    tagName: 'root'
  }
  /** Plot element of the chart. */
  interface Plot extends Element {
    tagName: 'plot'
  }
  /** Main title element of the chart. */
  interface Title extends TextElement {
    tagName: 'title'
  }
  /** Legend element of the chart. */
  interface Legend extends Element {
    tagName: 'legend'
    channel: string
  }
  /** Logo element of the chart. */
  interface Logo extends Element {
    tagName: 'logo'
  }
  /** Plot area element of the chart. */
  interface Area extends Element {
    tagName: 'plot-area'
  }
  /** Plot marker element of the chart representing a data point. */
  interface Marker extends Element {
    tagName: 'plot-marker'
    categories: Data.Record
    values: Data.Record
    /** Unique index of the marker. */
    index: number
  }
  /** Label element of a marker element. */
  interface MarkerLabel extends TextElement {
    tagName: 'plot-marker-label'
    parent: Marker
  }
  /** Guideline element of a marker element. */
  interface MarkerGuide extends OrientedElement {
    tagName: 'plot-marker-guide'
    parent: Marker
  }
  /** Axis element of the plot. */
  interface Axis extends OrientedElement {
    tagName: 'plot-axis'
  }
  /** Label element of an axis element. */
  interface AxisLabel extends TextElement {
    tagName: 'plot-axis-label'
    parent: Axis
  }
  /** Guideline element of an axis element. */
  interface AxisGuide extends Element {
    tagName: 'plot-axis-guide'
    parent: Axis
  }
  /** Tick element of an axis element. */
  interface AxisTick extends Element {
    tagName: 'plot-axis-tick'
    parent: Axis
  }
  /** Title element of an axis element. */
  interface AxisTitle extends TextElement {
    tagName: 'plot-axis-title'
    parent: Axis
  }
  /** Interlacing element of an axis element. */
  interface AxisInterlacing extends Element {
    tagName: 'plot-axis-interlacing'
    parent: Axis
  }
  /** Title element of a legend element. */
  interface LegendTitle extends TextElement {
    tagName: 'legend-title'
    parent: Legend
  }
  /** Marker element of a legend element. */
  interface LegendMarker extends Element {
    tagName: 'legend-marker'
    parent: Legend
  }
  /** Label element of a legend element. */
  interface LegendLabel extends TextElement {
    tagName: 'legend-label'
    parent: Legend
  }
  /** Bar element of a legend element indicating a continous interval. */
  interface LegendBar extends Element {
    tagName: 'legend-bar'
    parent: Legend
  }
  /** The interface of the event object is passed to event handlers by the library.
    Detail properties will vary by event type. */
  interface Event {
    type: Type
    /** The element of the chart the event triggered on. */
    target: Element | null
    /** If called, the default action of the event will be canceled. */
    preventDefault(): void
    /** For drawing events the rendering context of the underlying 
    canvas set up for drawing the element. */
    renderingContext?: CanvasRenderingContext2D
  }
  interface PointerDetail {
    pointerId: number | null
    position: Geom.Point
  }
  /** Event for pointer/mouse events. */
  interface PointerEvent extends Event {
    detail: PointerDetail
  }
  interface WheelDetail {
    delta: number
  }
  /** Event for mouse wheel change. */
  interface WheelEvent extends Event {
    detail: WheelDetail
  }
  interface UpdateDetail {
    position: Anim.Duration
    progress: number
  }
  /** Event emitted at every animation frame update. */
  interface UpdateEvent extends Event {
    detail: UpdateDetail
  }
  interface RectDrawDetail {
    rect: Geom.Rect
    relative: boolean
  }
  /** Event of drawing a rectangle or a marker 
    (in which case rect is the boundary rectangle). */
  interface RectDrawEvent extends Event {
    detail: RectDrawDetail
  }
  interface LineDrawDetail {
    line: Geom.Line
    relative: boolean
  }
  /** Event of drawing a line. */
  interface LineDrawEvent extends Event {
    detail: LineDrawDetail
  }
  interface TextDrawDetail {
    rect: Geom.TransformedRect
    text: string
  }
  /** Event of drawing a text. */
  interface TextDrawEvent extends Event {
    detail: TextDrawDetail
  }
  type Handler = (event: Event) => void
}
declare namespace Presets {
  interface Preset {
    legend?: 'color' | 'lightness' | 'size' | null
    title?: string | null
    reverse?: boolean
    sort?: 'none' | 'byValue'
  }
  interface Column extends Preset {
    x: string[] | string
    y: string[] | string
  }
  interface GroupedColumn extends Preset {
    groupedBy: string[] | string
    x: string[] | string
    y: string[] | string
  }
  interface StackedColumn extends Preset {
    x: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface SplittedColumn extends Preset {
    x: string[] | string
    y: string[] | string
    splittedBy: string[] | string
  }
  interface PercentageColumn extends Preset {
    x: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface Waterfall extends Preset {
    x: string[] | string
    y: string[] | string
  }
  interface Mekko extends Preset {
    x: string[] | string
    groupedBy: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface Marimekko extends Preset {
    x: string[] | string
    groupedBy: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface Bar extends Preset {
    x: string[] | string
    y: string[] | string
  }
  interface GroupedBar extends Preset {
    x: string[] | string
    groupedBy: string[] | string
    y: string[] | string
  }
  interface StackedBar extends Preset {
    x: string[] | string
    stackedBy: string[] | string
    y: string[] | string
  }
  interface SplittedBar extends Preset {
    x: string[] | string
    splittedBy: string[] | string
    y: string[] | string
  }
  interface PercentageBar extends Preset {
    x: string[] | string
    stackedBy: string[] | string
    y: string[] | string
  }
  interface Lollipop extends Preset {
    x: string[] | string
    y: string[] | string
  }
  interface Scatter extends Preset {
    x: string[] | string
    y: string[] | string
    dividedBy: string[] | string
  }
  interface Bubbleplot extends Preset {
    x: string[] | string
    y: string[] | string
    color: string[] | string
    size: string[] | string
    dividedBy: string[] | string
  }
  interface Area extends Preset {
    x: string[] | string
    y: string[] | string
  }
  interface StackedArea extends Preset {
    x: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface PercentageArea extends Preset {
    x: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface SplittedArea extends Preset {
    x: string[] | string
    y: string[] | string
    splittedBy: string[] | string
  }
  interface Stream extends Preset {
    x: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface VerticalStream extends Preset {
    x: string[] | string
    stackedBy: string[] | string
    y: string[] | string
  }
  interface Violin extends Preset {
    x: string[] | string
    y: string[] | string
    splittedBy: string[] | string
  }
  interface VerticalViolin extends Preset {
    x: string[] | string
    splittedBy: string[] | string
    y: string[] | string
  }
  interface Line extends Preset {
    x: string[] | string
    y: string[] | string
    dividedBy: string[] | string
  }
  interface VerticalLine extends Preset {
    x: string[] | string
    y: string[] | string
    dividedBy: string[] | string
  }
  interface Pie extends Preset {
    angle: string[] | string
    by: string[] | string
  }
  interface PolarColumn extends Preset {
    angle: string[] | string
    radius: string[] | string
  }
  interface PolarStackedColumn extends Preset {
    angle: string[] | string
    radius: string[] | string
    stackedBy: string[] | string
  }
  interface VariableRadiusPie extends Preset {
    angle: string[] | string
    by: string[] | string
    radius: string[] | string
  }
  interface RadialBar extends Preset {
    angle: string[] | string
    radius: string[] | string
  }
  interface RadialStackedBar extends Preset {
    angle: string[] | string
    stackedBy: string[] | string
    radius: string[] | string
  }
  interface Donut extends Preset {
    angle: string[] | string
    stackedBy: string[] | string
  }
  interface NestedDonut extends Preset {
    angle: string[] | string
    stackedBy: string[] | string
    radius: string[] | string
  }
  interface PolarScatter extends Preset {
    angle: string[] | string
    radius: string[] | string
    dividedBy: string[] | string
  }
  interface PolarLine extends Preset {
    angle: string[] | string
    radius: string[] | string
    dividedBy: string[] | string
  }
  interface Treemap extends Preset {
    size: string[] | string
    color: string[] | string
  }
  interface StackedTreemap extends Preset {
    size: string[] | string
    dividedBy: string[] | string
    color: string[] | string
  }
  interface Heatmap extends Preset {
    x: string[] | string
    y: string[] | string
    lightness: string[] | string
  }
  interface Bubble extends Preset {
    size: string[] | string
    color: string[] | string
  }
  interface StackedBubble extends Preset {
    size: string[] | string
    stackedBy: string[] | string
    color: string[] | string
  }
  /** Collection of factory functions for creating preset chart configs. */
  interface Presets {
    column(config: Column): Config.Chart
    groupedColumn(config: GroupedColumn): Config.Chart
    stackedColumn(config: StackedColumn): Config.Chart
    splittedColumn(config: SplittedColumn): Config.Chart
    percentageColumn(config: PercentageColumn): Config.Chart
    waterfall(config: Waterfall): Config.Chart
    mekko(config: Mekko): Config.Chart
    marimekko(config: Marimekko): Config.Chart
    bar(config: Bar): Config.Chart
    groupedBar(config: GroupedBar): Config.Chart
    stackedBar(config: StackedBar): Config.Chart
    splittedBar(config: SplittedBar): Config.Chart
    percentageBar(config: PercentageBar): Config.Chart
    lollipop(config: Lollipop): Config.Chart
    scatter(config: Scatter): Config.Chart
    bubbleplot(config: Bubbleplot): Config.Chart
    area(config: Area): Config.Chart
    stackedArea(config: StackedArea): Config.Chart
    percentageArea(config: PercentageArea): Config.Chart
    splittedArea(config: SplittedArea): Config.Chart
    stream(config: Stream): Config.Chart
    verticalStream(config: VerticalStream): Config.Chart
    violin(config: Violin): Config.Chart
    verticalViolin(config: VerticalViolin): Config.Chart
    line(config: Line): Config.Chart
    verticalLine(config: VerticalLine): Config.Chart
    pie(config: Pie): Config.Chart
    polarColumn(config: PolarColumn): Config.Chart
    polarStackedColumn(config: PolarStackedColumn): Config.Chart
    variableRadiusPie(config: VariableRadiusPie): Config.Chart
    radialBar(config: RadialBar): Config.Chart
    radialStackedBar(config: RadialStackedBar): Config.Chart
    donut(config: Donut): Config.Chart
    nestedDonut(config: NestedDonut): Config.Chart
    polarScatter(config: PolarScatter): Config.Chart
    polarLine(config: PolarLine): Config.Chart
    treemap(config: Treemap): Config.Chart
    stackedTreemap(config: StackedTreemap): Config.Chart
    heatmap(config: Heatmap): Config.Chart
    bubble(config: Bubble): Config.Chart
    stackedBubble(config: StackedBubble): Config.Chart
  }
}
declare namespace Plugins {
  interface PluginMeta {
    name: string;
    version?: string;
    depends?: string[];
  }

  interface SetAnimParamsContext {
    target: AnimTarget;
    options?: Anim.ControlOptions | (Anim.ControlOptions & Anim.Options);
  }

  interface AnimateRegisterContext {
    target: AnimTarget;
    options?: Anim.ControlOptions | (Anim.ControlOptions & Anim.Options);
    promise: Anim.Completing;
  }

  interface HookContexts {
    [Hooks.setAnimParams]: SetAnimParamsContext
    [Hooks.animateRegister]: AnimateRegisterContext
  }

  type PluginHook<T> = {
    (ctx: T, next: () => void): void;
    priority?: number;
  };

  type PluginHooks<T extends Hooks> = {
    [key in T]?: PluginHook<HookContexts[key]>
  };

  interface PluginListeners {
    [event: Event.Type]: (eventObj: any) => void;
  }

  interface PluginApi {
    [apiMethod: string]: (...args: any[]) => any;
  }

  interface Plugin {
    meta: PluginMeta;
    hooks?: PluginHooks;
    listeners?: PluginListeners;
    api?: PluginApi;
    register?: (ctx: Vizzu) => void;
    unregister?: (ctx: Vizzu) => void;
    enable?: (enabled: boolean) => void;
  }

  interface PluginApis {
    [name: string]: PluginApi;
  }
}

declare enum Hooks {
  setAnimParams = 'setAnimParams',
  animateRegister = 'animateRegister'
}
/** Stored Animation object. */
declare class CAnimation { }
/** Stored Chart object. */
declare class Snapshot { }
/** List of base and additional features:
    - logging: enables logging of the library to the console 
      (switched off by default).
    - rendering: enables rendering of the library to the canvas
      (enabled by default). 
    - tooltip: tooltips on the chart appearing on markers on mouse over. 
      Since the tooltip uses the animation interface, calling animate() while
      the tooltip is enabled can cause unwanted behaviour. */
type Feature = 'tooltip' | 'logging' | 'rendering'
/** Converts a point between two coordinate system. */
type Converter = (point: Geom.Point) => Geom.Point
/** Type of a point's coordinate system. */
type CoordinateType = 'relative' | 'canvas'
//import ObjectRegistry from './objregistry'
/** Class representing a single chart in Vizzu. */

interface VizzuOptions {
  container: HTMLElement
  features?: Plugins.Plugin[]
}
interface Vizzu {
  /** Creates a new chart and connects it to the div or canvas HTML 
    element specified by its ID or DOM object. The new chart is empty by 
    default, but can be set to an initial state in the second optional 
    parameter. */
  //  constructor(container: string | HTMLElement | VizzuOptions, initState?: Anim.Target | Config.Chart)
  /** Promise representing the initialization will resolve when 
    initialization is finished. Any API call will potentially cause 
    an error before this promise is resolved.  */
  initializing: Promise<Vizzu>
  /** Installs the provided event handler to the event specified by name. */
  on(eventName: Event.Type, handler: Event.Handler): void
  /** Uninstalls the provided event handler from the event specified by name. */
  off(eventName: Event.Type, handler: Event.Handler): void
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
    animTarget: AnimTarget,
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
  getConverter(target: 'plot-area', from: CoordinateType, to: CoordinateType): Converter
  /** Re-renders the chart. */
  forceUpdate(): void
  /** Property for read-only access to style object without default values. */
  style: Readonly<Styles.Chart>
  /** Property for read-only access to the style object after setting defaults. */
  getComputedStyle(): Readonly<Styles.Chart>
  /** Property for read-only access to chart parameter object. */
  config: Readonly<Config.Chart>
  /** Property for read-only access to data metainfo object. */
  data: Readonly<Data.Metainfo>
  /** Enable/disable additional features. */
  feature(name: Feature, enabled: boolean): void
  /** Removes the reference of the chart from every place it attached itself,
    this method must be called in order to get the chart properly garbage 
    collected.  */
  detach(): void
  /** Returns the chart preset collection. */
  //  static get presets(): Presets.Presets
  /** Setter method for Library options. */
  //  static options(options: Lib.Options): void
}
