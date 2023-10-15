/* eslint-disable @typescript-eslint/no-unused-vars, lines-between-class-members, no-use-before-define */

export namespace Styles {
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
    /** Style settings for the main chart subtitle. */
    subtitle?: Label | null
    /** Style settings for the main chart caption. */
    caption?: Label | null
    /** Style settings for the tooltip. */
    tooltip?: Tooltip | null
    /** Style settings of the Vizzu logo. */
    logo?: Logo | null
  }
}
