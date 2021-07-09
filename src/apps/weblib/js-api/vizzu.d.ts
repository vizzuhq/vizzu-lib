interface DataSeries {
	name: string,
	type: 'categories'|'values';
	values: string[]|number[];
}

interface DataSet {
	series: DataSeries[];
	filter: (record: any) => boolean;
}

interface Channel {
	title: string|null;
	attach: string[];
	detach: string[];
	range: string;
	labelLevel: number;
}

interface Descriptor {
	channels: {
		x: Channel;
		y: Channel;
		color: Channel;
		lightness: Channel;
		size: Channel;
		shape: Channel;
		label: Channel;
	};
	title: string|null;
	legend: 'color'|'lightness'|'size'|null;
	coordSystem: 'cartesian'|'polar';
	rotation: number;
	geometry: 'rectangle'|'circle'|'area'|'line';
	orientation: 'horizontal'|'vertical';
	sort: 'none'|'experimental';
	reverse: boolean;
	align: 'none'|'min'|'center'|'max'|'stretch';
	split: boolean;
}

interface Padding {
	paddingTop: number|string;
	paddingRight: number|string;
	paddingBottom: number|string;
	paddingLeft: number|string;
}

interface Font {
	fontFamily: string;
	fontStyle: 'normal'|'italic'|'oblique';
	fontWeight: 'normal'|'bold'|number;
	fontSize: number|string;
}

interface Box {
	backgroundColor: string;
	borderColor: string;
	borderWidth: number;
}

interface Text {
	color: string;
	textAlign: 'center'|'left'|'right';
	backgroundColor: string;
	overflow: 'visible'|'hidden';
	numberFormat: 'none'|'groupped'|'prefixed';
}

interface MarkerLabel extends Label {
	position: 'below'|'center'|'above';
	filter: string;
	format: 'valueFirst'|'categoriesFirst';
}

interface Marker {
	borderWidth: number;
	borderOpacity: number;
	borderOpacityMode: 'straight'|'premultiplied';
	fillOpacity: number;
	guides: {
		color: string;
		lineWidth: number;
	};
	label: MarkerLabel;
}

interface Axis {
	color: string;
	title: Label;
	label: Label;
	ticks: {
		color: string;
		lineWidth: number;
		length: number|string;
		position: 'outside'|'inside'|'center';
	};
	guides: {
		color: string;
		lineWidth: number;
	};
	interlaceing: {
		color: string;
	};
}

interface Plot extends Padding, Box {
	marker: Marker;
	axis: Axis;
}

interface Legend extends Padding, Box {
	width: number|string;
	title: Label;
	label: Label;
	marker: {
		type: 'circle'|'square';
		size: number|string;
	};
}

interface Data {
	colorGradient: string;
	colorPalette: string;
	minLightness: number;
	maxLightness: number;
	lineWidth: number;
	lineMinWidth: number;
	lineMaxWidth: number;
	circleMinRadius: number;
	circleMaxRadius: number;
	barMaxPadding: number;
	barPaddingDecrease: number;
	columnMaxPadding: number;
	columnPaddingDecrease: number;
}

type Label = Padding & Font & Text;

interface Styles extends Padding, Box, Font {
	plot: Plot;
	legend: Legend;
	title: Label;
	data: Data;
}

interface AnimationTarget {
	data: DataSet;
	descriptor: Descriptor;
	style: Styles;
}

interface AnimControl {
    seek(value: string): void;
    pause(): void;
    play(): void;
    stop(): void;
    reverse(): void;
}

export default class Vizzu {
    constructor(container: any);
    initializing: Promise<Vizzu>;
    on(eventName: string, handler: any): void;
    off(eventName: string, handler: any): void;
    animate(obj: AnimationTarget): Promise<Vizzu>;
    get animation(): AnimControl;
    version(): string;
}
