declare namespace Lib 
{

/** Options for the library. */
interface Options {

	/** The URL for the webassembly binary (cvizzu.wasm). */
	wasmUrl?: string;

}

}

declare namespace Data
{

/** Additional info about a data series besides the contained data. */
interface SeriesMetaInfo
{
	/** Name of the data series. It will be the unique id of the series to 
	    reference it in various parts of the API, mainly in {@link Channel} and
	    {@link Data.Record}. This name will also be used by default for Axis and 
	    Legend title. */
	name: string;
	/** Type of the data series:
	    - 'dimension' - categorical data containing strings 
		(dates should also be added as strings); 
	    - 'measure' - continuous data containing numbers. 
	    If not set, the library will attempt to determine the type based on 
	    the type of the first value. Number type will result in measure, 
	    string type will result in dimension. */
	type?: 'dimension'|'measure'; 
	/** Unit of the data series */
	unit?: string;
}

interface AbstractSeriesInfo extends SeriesMetaInfo
{
	/** Count of values in the series. */
	length: number;
}

/** Meta data about dimension data series */
interface DimensionSeriesInfo extends AbstractSeriesInfo
{
	/** Distinct values in the series */
	categories: Array<string>;
}

/** Meta data about measure data series */
interface MeasureSeriesInfo extends AbstractSeriesInfo
{
	range: {
		/** Minimal value in the series */
		min: number;
		/** Maximal value in the series */
		max: number;
	}
}

type SeriesInfo = DimensionSeriesInfo|MeasureSeriesInfo;

/** Meta data about the data set */
interface Metainfo
{
	series: SeriesInfo[];
}

/** Represents a categorical or data value */
type Value = string|number;

/** List of data values in a series. */
type Values = string[]|number[];

/** Defines a data series of the data set, and contains a particular variable's
    values in the data set and meta info about the variable. */
interface Series extends SeriesMetaInfo {
	/** The array that contains the values of the data series. The value types 
	    should match {@link Data.SeriesMetaInfo.type}. If the data series
	    is shorter than the longest data series defined, it will be internally 
	    extended with empty values. */
	values: Values;
}

/** A record of the data set, containing one value of each data series 
    corresponding to the same index. */
interface Record {
	/** Properties are provided for each data series, providing access to the value within
	    the record referenced by its {@link Data.Series.name|name}. */
	[seriesName: Series["name"]]: Value;
}

type FilterCallback = (record: Record) => boolean;

interface Filter {
	/** A filter callback is called on each record of the dataset on chart
	    generation. If the callback returns false, the record will not be shown on the chart.
	 */
	filter?: FilterCallback | null;
}

/** Data table specified by series. */
interface TableBySeries extends Filter
{
	/** The series that make up the the data set. */
	series: Series[];
}

/** Data table specified by records. */
interface TableByRecords extends Filter
{
	/** The information about the data series in the records of the data set. 
	    Note: not needed if it was previously specified. */
	series?: SeriesMetaInfo[];
	/** The array of data records that make up the data set. */
	records: Value[][];
}

type CubeRow = Values|CubeRow[];

/** Defines a data series of the data cube, and contains a particular variable's
    values in the data cube and meta info about that variable. */
interface CubeData extends SeriesMetaInfo {
	/** A nested array that contains the values of the data series. Nesting 
	    level should match the number of {@link Data.Cube.dimensions}. */
	values: CubeRow;
}

/** N dimensional data cude */
interface Cube extends Filter
{
	/** The list of the dimensions of the data cube. */
	dimensions?: Series[];
	/** The list of measures of the data cube. */
	measures?: CubeData[];
}

/** Data set is a collection of related {@link Data.Series|data series}. 
    Each chart works on a single data set. */
type Set = TableBySeries|TableByRecords|Cube;

type SeriesList = string[]|string;

}

declare namespace Config
{

/* Units: 
   - no unit: the same unit as in the data;
   - %: percentage relative to the min/max of the data; 
   - min,max: offset from min/max of the data;
   - auto: automatic range based on chart config;
 */
type ChannelExtrema = number|`${number}%`|`${number}min`|`${number}max`|'auto';

/** Channel range specifies how to scale the represented data. */
interface ChannelRange {
	min?: ChannelExtrema|null;
	max?: ChannelExtrema|null;
}

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
	title?: string|'auto'|null;
	/** List of {@link Data.Series.name|data series names} on the 
	    channel. */
	set? : Data.SeriesList|null;
	/** List of {@link Data.Series.name|data series names} to be added to the 
	    channel beside the ones already added. */
	attach?: Data.SeriesList;
	/** List of {@link Data.Series.name|data series names} to be removed from the 
	    channel. */
	detach?: Data.SeriesList;
	/** Specifies the range that determines how the represented data scales 
		on the channel. */
	range?: ChannelRange;
	/** Only one dimension can be shown on an axis or legend by
	    name. This index specifies which attached series should be used. */
	labelLevel?: number;
}

/** Channel configuration. 
	A data series' name or a list of the data series' names can be used as a 
	short-hand - instead of the {@link Channel|channel object} - to set data series 
	for the channel. Setting a channel to null will remove all data series from it. */
interface Channels {
	/** Parameters for the X-axis, determining the position of the markers on the 
	    x-axis - or their angle when using polar coordinates. 
	    Note: leaving x and y channels empty will result in a 
	    chart "without coordinates" like a Treemap or a Bubble Chart. */
	x?: Channel|Data.SeriesList|null;
	/** Parameters for the Y-axis, determining the position of the markers on the 
	    y-axis - or their radius when using polar coordinates) . */
	y?: Channel|Data.SeriesList|null;
	/** Parameters for the markers' base color. The markers' actual color can 
	    also be affected by the lightness channel. */
	color?: Channel|Data.SeriesList|null;
	/** Parameters for markers' lightness. */
	lightness?: Channel|Data.SeriesList|null;
	/** Parameters for the markers' size. Effective only for circle and line
	    geometry affecting the circle area or the line width respectively.
	    */
	size?: Channel|Data.SeriesList|null;
	/** Parameters for the content of the labels that appear on the markers. */
	label?: Channel|Data.SeriesList|null;
	/** Splits the markers as all the other channels, but have no 
	    effect on the markers' appearance. Thus, it only works with dimensions. */
	noop?: Channel|Data.SeriesList|null;
}

/** The config contains all of the parameters needed to render a particular 
    static chart or a state of an animated chart. */
interface Chart extends Channels {
	/** List of the chart's channels' configuration. The chart object also 
	    extends the channels object as a configuration shorthand. */
	channels?: Channels;
	/** This is the title shown on the top of the chart.
	    If set to null, the title will not be shown and will not take up any
	    space in the chart layout. */
	title?: string|null;
	/** Specifies which channel should be shown on the legend.
	    If set to null, the legend will not be shown and will not take up any
	    space in the chart layout. */
	legend?: 'color'|'lightness'|'size'|null;
	/** Sets the coordinate system for the chart. Switch to the 'polar' 
	    coordinate system to create a chart from the pie/radial chart family. */
	coordSystem?: 'cartesian'|'polar';
	/** Rotates the plot area by the specified angle in degree. 
	    Note: this is an experimental, not tested feature. */
	rotate?: number;
	/** Sets the geometric elements used for the markers to represent the data.*/
	geometry?: 'rectangle'|'circle'|'area'|'line';
	/** If both axes have measures on them, this parameter sets the 
	    orientation of the chart, meaning to which axis the graphical elements 
	    are oriented to. */
	orientation?: 'horizontal'|'vertical';
	/** - 'none': markers are sorted in the order as the corresponding data 
	              appear in the data set.
	    - 'byValue': markers will be sorted by the corresponding measure (if present)
		in decreasing order. */
	sort?: 'none'|'byValue';
	/** Reverts the order of the markers if set. */
	reverse?: boolean;
	/** Sets the alignment of the markers with relation to the x- or the y-axis depending
	on where the measure is. In case both axes have measures on them, this is determined 
	by the {@link Chart.orientation|orientation} of the chart.
	*/
	align?: 'none'|'min'|'center'|'max'|'stretch';
	/** If set to true, markers will be split by the dimension(s) along the axis.
	This works if you have at least one dimension and a measure on the same axis.In case 
	both axes have measures and dimension(s) on them, this is determined by the 
	{@link Chart.orientation|orientation} of the chart.*/ 
	split?: boolean;
}

}

declare namespace Styles
{

/** Length can be set in pixels or in percentage of the element or the element's
    font size. Pixel is the default unit. */
type Length = `${number}px`|`${number}%`|`${number}em`|number;

/** Angle can be set in radians, degrees, gradians and turns. 
 	Radians is the default unit. */
type Angle = `${number}rad`|`${number}grad`|`${number}deg`|`${number}turn`
	|number;

/** The following CSS color formats are available: 
    rgb(), rgba(), #RRGGBB, #RRGGBBAA, #RGB. */
type Color = `#${string}`
	|`rgb(${number},${number},${number})`
	|`rgba(${number},${number},${number},${number})`;

interface Padding {
	/** Top padding of the element. */
	paddingTop?: Length|null;
	/** Right padding of the element. */
	paddingRight?: Length|null;
	/** Bottom padding of the element. */
	paddingBottom?: Length|null;
	/** Left padding of the element. */
	paddingLeft?: Length|null;
}

interface Font {
	/** The family of the font. If not set, it inherits the root style font
	    family. */
	fontFamily?: string|null;
	/** The style of the font. */
	fontStyle?: 'normal'|'italic'|'oblique'|null;
	/** The weight of the font, numbers use the same scale as CSS. */
	fontWeight?: 'normal'|'bold'|number|null;
	/** The size of the font. Percentage values are relative to the root style font
	    size */
	fontSize?: Length|null;
}

interface Box {
	/** The background color of the element. */
	backgroundColor?: Color|null;
	/** The border color of the element. */
	borderColor?: Color|null;
	/** The border width of the element. */
	borderWidth?: number|null;
}

interface Text {
	/** The color of the displayed text. */
	color?: Color|null;
	/** The alignment of the displayed text. */
	textAlign?: 'center'|'left'|'right'|null;
	/** The background color of the displayed text. */
	backgroundColor?: Color|null;
	/** The format of the number. Only applicable for texts showing numerical
	    data such as marker and axis labels. 'grouped' uses thousand separators, 
		'prefixed' uses scientific notation. */
	numberFormat?: 'none'|'grouped'|'prefixed'|null;
	/** The maximum number of digits in fraction part if the text contains a 
	    number. */
	maxFractionDigits?: number|null;
}

/** The following CSS like filters can be used to alter the color: 
    
    - color: overrides the color.
    - lightness: lightens or darkens the color; 0 means the original color, -1 
                 means black, 1 means white.
    - grayscale: desaturates the color. 0 means the original color, 1 means fully
                 desaturated.
   
    none: no change.
 */
type ColorTransform = `color(${Color})`
	| `lightness(${number})`
	| `grayscale(${number})`
	| `opacity(${number})`
	| 'none';

interface OrientedLabel extends Label {
	/** Orientation of the label relatively to the axis or marker it is attached to. */
	orientation?: 'normal'|'tangential'|'horizontal'|'vertical'|null;
	/** Additional rotation of the label. */
	angle?: Angle|null;
} 

interface MarkerLabel extends OrientedLabel {
	/** The label position relatively to the marker. */
	position?: 'center'|'top'|'left'|'bottom'|'right'|null;
	/** Defines the transformation used for calculating the label color
		from the marker color. */
	filter?: ColorTransform|null;
	/** Sets the order of values on the label if both a measure and a dimension are
	    present. */
	format?: 'measureFirst'|'dimensionsFirst'|null;
}

interface Guides {
	/** The color of the guides.*/
	color?: Color|null;
	/** Line width of the guide in pixel. */
	lineWidth?: number|null;
}

interface Tooltip extends Font, Box {
	/** The layout of the tooltip text */
	layout?: 'singleLine'|'multiLine';
	/** The foreground color of the tooltip text */
	color?: Color;
	/** Color of the drop shadow */
	shadowColor?: Color;
	/** Corner radius for the info bubble */
	borderRadius?: number;
	/** Drop shadow distance from the info bubble */
	dropShadow?: number;
	/** Data point marker radius */
	radius?: number;
	/** Base size of the info bubble arrow */
	arrowSize?: number;
	/** Distance between the data point and the info bubble */
	distance?: number;
	/** Specifies the name of the data series dispalyed
	    at the first position on the tooltip */
	seriesName?: string|null;
}

interface Logo extends Padding
{
	/** Width of the Vizzu logo */
	width?: Length|null;
	/** Color transformation applied on the colored Vizzu Logo */
	filter?: ColorTransform|null;
}

interface DataPoint {
	/** Color gradient used for the measure on the color channel.*/
	colorGradient?: ColorGradient|null;
	/** Color palette used for the dimension on the color channel.*/
	colorPalette?: ColorPalette|null;
	/** Lightness value associated with the minimum value of the lightness 
	    channel's range. */
	minLightness?: number|null;
	/** Lightness value associated with the maximum value of the lightness 
	    channel's range. */
	maxLightness?: number|null;
	/** Minimum of line width specified as proportion of plot area size. 
		e.g.: 0.01 means 1% of the width of the plot area. */
	lineMinWidth?: number|null;
	/** Maximum line width specified as proportion of plot area size.
		e.g.: 0.01 means 1% of the width of the plot area. */
	lineMaxWidth?: number|null;
	/** Minimum circle radius specified as proportion of plot area size.
		e.g.: 0.01 means 1% of the width of the plot area. */
	circleMinRadius?: number|null;
	/** Maximum circle radius specified as proportion of plot area size. 
		e.g.: 0.01 means 1% of the width of the plot area. */
	circleMaxRadius?: number|null;
	/** Spacing between bars/columns. The value specifies the size of the 
	    spacing as a factor of the marker size. 
		e.g.: 0.1 means 10% of marker height/width depending on the chart's orientation. */
	rectangleSpacing?: number|null;
}

interface Marker extends DataPoint {
	/** Width of the marker border in pixels. */
	borderWidth?: number|null;
	/** Opacity of the marker border. */
	borderOpacity?: number|null;
	borderOpacityMode?: 'straight'|'premultiplied'|null;
	/** Opacity of the marker fill color. */
	fillOpacity?: number|null;
	/** Style settings for guide lines drawn for the markers. */
	guides?: Guides|null;
	/** Style settings for the marker labels. */
	label?: MarkerLabel|null;
}

	/** Style settings for the values shown on the axis to display the scale 
		being used or the categories along the axis. */
interface AxisLabel extends OrientedLabel {
	/** Label position relatively to the plot. */
	position?: 'axis'|'max-edge'|'min-edge'|null;
	/** Label alignment relatively to the position on the plot. */
	side?: 'positive'|'negative'|null;
}

	/** Style settings of the {@link Config.Channel.title|Axis title} */
interface AxisTitle extends Label {
	/** Title position relatively to the plot. */
	position?: 'axis'|'min-edge'|'max-edge'|null;
	/** Title alignment relatively to the position on the plot. */
	side?: 'positive'|'upon'|'negative'|null;
	/** Title position on the axis or edge. */
	vposition?: 'begin'|'middle'|'end'|null;
	/** Title alignment on the axis or edge. */
	vside?: 'positive'|'upon'|'negative'|null;
	/** The orientation of the title. */
	orientation?: 'horizontal'|'vertical'|null;
}

interface Ticks {
	/** Color of the ticks on the axis. */
	color?: Color|null;
	/** Line width of the ticks on the axis. */
	lineWidth?: number|null;
	/** Length of the ticks on the axis. */
	length?: Length|null;
	/** Position of the ticks on the axis relatively to the axis line. */
	position?: 'outside'|'inside'|'center'|null;
} 
interface Interlacing {
	/** Color of the interlacing pattern. */
	color?: Color|null;
}

interface Axis {
	/** Color of the axis line. */
	color?: Color|null;
	/** Style parameters of the axis title. */
	title?: AxisTitle|null;
	/** Style parameters of the axis labels. */
	label?: AxisLabel|null;
	ticks?: Ticks|null;
	guides?: Guides|null;
	interlacing?: Interlacing|null;
}

interface Plot extends Padding, Box {
	/** Style settings for the markers. */
	marker?: Marker|null;
	/** Style settings for the x-axis - or the angle when using polar coordinates. */
	xAxis?: Axis|null;
	/** Style settings for the y-axis - or the radius when using polar coordinates. */
	yAxis?: Axis|null;
	/** Controls drawing outside of the plot area. If hidden, clipping will be
	    set for the boundary of the coordinate system. */
	overflow?: 'hidden'|'visible';
}

interface LegendMarker {
	/** Shape of the legend marker. */
	type?: 'circle'|'square'|null;
	/** Size of the legend marker (diameter or side length). */
	size?: Length|null;
}

interface Legend extends Padding, Box {
	/** Width of the legend's boundary box. */
	width?: Length|null;
	/** Limit for the width of the boundary box. */
	maxWidth?: Length|null;
	/** Style settings for the legend's title. */
	title?: Label|null;
	/** Style settings for the labels on the legend. */
	label?: Label|null;
	marker?: LegendMarker|null;
}

	/** Color and position pairs separated by spaces, 
		where position is a number between 0 and 1.  */
type ColorStop = `${Color} ${number}`;

/** Color gradient is specified by a comma separated list of ColorStops. 
	This is used when a measure is on the color channel. */
type ColorGradient = ColorStop 
	| `${ColorStop},${ColorStop}`
	| `${ColorStop},${ColorStop},${ColorStop}`
	| `${ColorStop},${ColorStop},${ColorStop},${ColorStop}`
	| `${ColorStop},${ColorStop},${ColorStop},${ColorStop},${ColorStop}`
	| string;

/** Color palette is a list of colors separated by spaces. 
 	This is used when only dimensions are on the color channel*/
type ColorPalette = Color 
	| `${Color} ${Color}` 
	| `${Color} ${Color} ${Color}`
	| `${Color} ${Color} ${Color} ${Color}`
	| `${Color} ${Color} ${Color} ${Color} ${Color}`
	| string;

type Label = Padding & Font & Text;

interface Chart extends Padding, Box, Font {
	/** Style settings for the plot area. */
	plot?: Plot|null;
	/** Style settings for the legend. */
	legend?: Legend|null;
	/** Style settings for the main chart title. */
	title?: Label|null;
	/** Style settings for the tooltip. */
	tooltip?: Tooltip|null;
	/** Style settings of the Vizzu logo. */
	logo?: Logo|null;
}

}


/** Represents a state in the animation describing the data, the chart, and 
    the style parameters to be changed from the actual state.
    Passing null as style will reset every style parameter to default. */
interface AnimTarget {
	/** Data set. */
	data?: Data.Set;
	/** Chart configuration changes. */
	config?: Config.Chart;
	/** Style changes. */
	style?: Styles.Chart|null;
}

declare namespace Anim
{

/** Duration can be set in seconds or milliseconds. 
    In case no unit is set, it defaults to seconds. */
type Duration = `${number}s`|`${number}ms`|number;

type Easing = 'none' | 'linear' | 'step-start' | 'step-end' | 'ease'
	| 'ease-in' | 'ease-out' | 'ease-in-out'
	| `cubic-bezier(${number},${number},${number},${number})`;

/** Animation parameters for an animation group. */
interface GroupOptions 
{
	/** Sets the easing used for the animation. */
	easing?: Easing;
	/** The length of time an animation should take to complete. */
	duration?: Duration;
	/** Waiting time interval before the animation starts. */
	delay?: Duration;
}

/** If no animation settings are passed to Vizzu, it will use an automatic 
    setting depending on the actual configuration of the chart. This behavior can be
    overridden via the animation setting parameter.
   
    The animation between two states of the chart can require the transitioning
    of several different chart properties. These properties are grouped into 
    separately configurable animation groups.
  
    The parameters can also be set for the animation as a whole. These settings
    rescale the durations and delays of the animation groups to the 
    specified total delay and duration. 
 */
interface Options extends GroupOptions {
	/** Determines if the animation should start automatically after the 
	    animate() call. */
	playState?: 'paused'|'running';
	/** The starting position of the animation. */
	position: number;
	/** Animation group for style parameters. */
	style?: GroupOptions;
	/** Title animation parameters. */
	title?: GroupOptions;
	/** Legend animation parameters. */
	legend?: GroupOptions;
	/** Animation group for new markers fading in 
	    (due to filtering or added/removed data series). */
	show?: GroupOptions;
	/** Animation group for markers fading out 
	    (due to filtering or added/removed data series). */
	hide?: GroupOptions;
	/** Marker color animation group. */
	color?: GroupOptions;
	/** Coordinate system transformations animation group. */
	coordSystem?: GroupOptions;
	/** Marker geometry morph animation group. */
	geometry?: GroupOptions;
	/** Animation group for marker transitions in the direction of the y-axis. */
	y?: GroupOptions;
	/** Animation group for marker transitions in the direction of the x-axis. */
	x?: GroupOptions;
	/** Animation group for tooltip transitions. */
	tooltip?: GroupOptions;
}

/** Control object for animation. */
interface Control extends Promise<Vizzu> {
	/** Seeks the animation to the position specified by time or progress 
	    percentage. Seeking the animation to the end position will not trigger
	    the (@link Vizzu.animate|animation promise) to resolve. */
	seek(value: `${number}%`|Duration): void;
	/** Pauses the controlled animation. */
	pause(): void;
	/** Plays/resumes playing of the controlled animation. */
	play(): void;
	/** Stops the current animation seeking it back to its start position. */
	stop(): void;
	/** Changes the direction of the controlled animation. */
	reverse(): void;
	/** Cancels the animation, will reject the animation promise. */
	cancel(): void;
}

}

declare namespace Event
{

type Type =
	 'click'
	|'mouseon'
	|'mousemove'
	|'wheel'
	|'update'
	|'background-draw'
	|'title-draw'
	|'logo-draw'
	|'legend-background-draw'
	|'legend-title-draw'
	|'legend-label-draw'
	|'legend-marker-draw'
	|'legend-bar-draw'
	|'plot-background-draw'
	|'plot-marker-draw'
	|'plot-marker-label-draw'
	|'plot-marker-guide-draw'
	|'plot-axis-draw'
	|'plot-axis-title-draw'
	|'plot-axis-label-draw'
	|'plot-axis-tick-draw'
	|'plot-axis-guide-draw'
	|'plot-axis-interlacing-draw'
	|'animation-begin'
	|'animation-complete';

/** The interface of the event object is passed to event handlers by the library.
    Additional properties will vary by event type. */
interface Object {
	/** If called, the default action of the event will be canceled. */
	preventDefault: () => void;
}

}

type Snapshot = number;

/** List of base and additional features:
    - logging: enables logging of the library to the console 
      (switched off by default).
    - rendering: enables rendering of the library to the canvas
      (enabled by default). 
    - tooltip: tooltips on the chart appearing on markers on mouse over. 
      Since the tooltip uses the animation interface, calling animate() while
      the tooltip is enabled can cause unwanted behaviour. */
type Feature = 'tooltip';

/** Class representing a single chart in Vizzu. */
export default class Vizzu {
	/** Creates a new chart and connects it to the div or canvas HTML 
	    element specified by its ID or DOM object. The new chart is empty by 
	    default, but can be set to an initial state in the second optional 
	    parameter. */
	constructor(container: string|HTMLElement, initState?: AnimTarget|Config.Chart);
	/** Promise representing the initialization will resolve when 
	    initialization is finished. Any API call will potentially cause 
	    an error before this promise is resolved. */
	initializing: Promise<Vizzu>;
	/** Installs the provided event handler to the event specified by name. */
	on(eventName: Event.Type, handler: (event: Event.Object) => void): void;
	/** Uninstalls the provided event handler from the event specified by name.
	 */
	off(eventName: Event.Type, handler: (event: Event.Object) => void): void;
	/** Initiates the animation to the new chart state passed as the first 
	    argument. If there is a currently running animation, all subsequent 
	    calls will schedule the corresponding animation after the end of the 
	    previous one.

	    The new chart state can be a full state specifier object with 
	    data, config and style, or a single chart config object.
	    It accepts also a chart snapshot acquired from a previous state using 
	    the store() method.

	    The optional second parameter specifies the animation 
	    options. This second option can be a scalar value, setting the overall 
	    animation duration. Passing explicit null as second parameter will
	    result in no animation.

	    The animation will be initiated in the next cycle of the JS event loop.
	    The method returns a promise, which will resolve when the animation is
	    finished. */
	animate(
		animTarget: AnimTarget|Config.Chart|Snapshot, 
		animOptions?: Anim.Options|Anim.Duration|null)
		: Anim.Control;
	/** Returns a reference to the actual chart state for further reuse. 
		This reference includes the chart config, style parameters and the
		data filter but does not include the actual data and the animation options.
		*/
	store(): Snapshot;
	/** Returns controls for the ongoing animation, if any.
	    @deprecated since version 0.4.0 */
	get animation(): Anim.Control;
	/** Returns the version number of the library. */
	version(): string;
	/** Property for read-only access to style object. */
	style: Readonly<Styles.Chart>;
	/** Property for read-only access to chart parameter object. */
	config: Readonly<Config.Chart>;
	/** Property for read-only access to data metainfo object. */
	data: Readonly<Data.Metainfo>;
	/** Enable/disable additional features. */
	feature(name: Feature, enabled: boolean): void;
	/** Returns the chart preset collection. */
	static get presets(): import('preset').Preset;
	/** Setter method for Library options. */
	static options(options: Lib.Options): void;
}
