import { CFunction } from './cvizzu.types'
import { PluginListeners } from './plugins.js'

import { CChart, CEvent } from './module/cchart.js'

import * as Data from './types/data.js'
import * as Anim from './types/anim.js'
import { Point, Rect, Line, TransformedRect } from './geom.js'

export enum EventType {
	click = 'click',
	pointermove = 'pointermove',
	pointerdown = 'pointerdown',
	pointerup = 'pointerup',
	pointerleave = 'pointerleave',
	wheel = 'wheel',
	update = 'update',
	drawBegin = 'draw-begin',
	drawComplete = 'draw-complete',
	backgroundDraw = 'background-draw',
	titleDraw = 'title-draw',
	subtitleDraw = 'subtitle-draw',
	captionDraw = 'caption-draw',
	logoDraw = 'logo-draw',
	legendBackgroundDraw = 'legend-background-draw',
	legendTitleDraw = 'legend-title-draw',
	legendLabelDraw = 'legend-label-draw',
	legendMarkerDraw = 'legend-marker-draw',
	legendBarDraw = 'legend-bar-draw',
	plotBackgroundDraw = 'plot-background-draw',
	plotAreaDraw = 'plot-area-draw',
	plotMarkerDraw = 'plot-marker-draw',
	plotMarkerLabelDraw = 'plot-marker-label-draw',
	plotMarkerGuideDraw = 'plot-marker-guide-draw',
	plotAxisDraw = 'plot-axis-draw',
	plotAxisTitleDraw = 'plot-axis-title-draw',
	plotAxisLabelDraw = 'plot-axis-label-draw',
	plotAxisTickDraw = 'plot-axis-tick-draw',
	plotAxisGuideDraw = 'plot-axis-guide-draw',
	plotAxisInterlacingDraw = 'plot-axis-interlacing-draw',
	animationBegin = 'animation-begin',
	animationComplete = 'animation-complete'
}

export interface EventMap {
	[EventType.click]: PointerEvent
	[EventType.pointermove]: PointerEvent
	[EventType.pointerdown]: PointerEvent
	[EventType.pointerup]: PointerEvent
	[EventType.pointerleave]: PointerEvent
	[EventType.wheel]: WheelEvent
	[EventType.update]: UpdateEvent
	[EventType.drawBegin]: Event<Element>
	[EventType.drawComplete]: Event<Element>
	[EventType.backgroundDraw]: RectDrawEvent<Root>
	[EventType.titleDraw]: TextDrawEvent<Title>
	[EventType.subtitleDraw]: TextDrawEvent<Subtitle>
	[EventType.captionDraw]: TextDrawEvent<Caption>
	[EventType.logoDraw]: RectDrawEvent<Logo>
	[EventType.legendBackgroundDraw]: RectDrawEvent<Legend>
	[EventType.legendTitleDraw]: TextDrawEvent<LegendTitle>
	[EventType.legendLabelDraw]: TextDrawEvent<LegendLabel>
	[EventType.legendMarkerDraw]: RectDrawEvent<LegendMarker>
	[EventType.legendBarDraw]: RectDrawEvent<LegendBar>
	[EventType.plotBackgroundDraw]: RectDrawEvent<Plot>
	[EventType.plotAreaDraw]: RectDrawEvent<Area>
	[EventType.plotMarkerDraw]: RectDrawEvent<Marker>
	[EventType.plotMarkerLabelDraw]: TextDrawEvent<MarkerLabel>
	[EventType.plotMarkerGuideDraw]: LineDrawEvent<MarkerGuide>
	[EventType.plotAxisDraw]: LineDrawEvent<Axis>
	[EventType.plotAxisTitleDraw]: TextDrawEvent<AxisTitle>
	[EventType.plotAxisLabelDraw]: TextDrawEvent<AxisLabel>
	[EventType.plotAxisTickDraw]: LineDrawEvent<AxisTick>
	[EventType.plotAxisGuideDraw]: LineDrawEvent<AxisGuide>
	[EventType.plotAxisInterlacingDraw]: RectDrawEvent<AxisInterlacing>
	[EventType.animationBegin]: Event<Element>
	[EventType.animationComplete]: Event<Element>
}

/** Abstract Element interface for chart elements. */
export interface Element {
	/** Name of the chart element type. */
	tagName: string
	/** Parent element of the chart element. */
	parent?: Element
}

/** Element with a string value. */
export interface TextElement extends Element {
	value: string
}

/** Element with an orientation value. */
export interface OrientedElement extends Element {
	id: 'x' | 'y'
}

/** Root element of the chart. */
export interface Root extends Element {
	tagName: 'root'
}

/** Plot element of the chart. */
export interface Plot extends Element {
	tagName: 'plot'
}

/** Main title element of the chart. */
export interface Title extends TextElement {
	tagName: 'title'
}

/** Subtitle element of the chart. */
export interface Subtitle extends TextElement {
	tagName: 'subtitle'
}

/** Caption element of the chart. */
export interface Caption extends TextElement {
	tagName: 'caption'
}

/** Legend element of the chart. */
export interface Legend extends Element {
	tagName: 'legend'
	channel: string
	scrollTop: number
	scrollHeight: number
}

/** Logo element of the chart. */
export interface Logo extends Element {
	tagName: 'logo'
}

/** Plot area element of the chart. */
export interface Area extends Element {
	tagName: 'plot-area'
}
/** Plot marker element of the chart representing a data point. */
export interface Marker extends Element {
	tagName: 'plot-marker'
	categories: Data.Record
	values: Data.Record
	/** Unique index of the marker. */
	index: string
}

/** Label element of a marker element. */
export interface MarkerLabel extends TextElement {
	tagName: 'plot-marker-label'
	parent: Marker
}

/** Guideline element of a marker element. */
export interface MarkerGuide extends OrientedElement {
	tagName: 'plot-marker-guide'
	parent: Marker
}

/** Axis element of the plot. */
export interface Axis extends OrientedElement {
	tagName: 'plot-axis'
}

/** Label element of an axis element. */
export interface AxisLabel extends TextElement {
	tagName: 'plot-axis-label'
	parent: Axis
	categories?: Data.Record
}

/** Guideline element of an axis element. */
export interface AxisGuide extends Element {
	tagName: 'plot-axis-guide'
	parent: Axis
}

/** Tick element of an axis element. */
export interface AxisTick extends Element {
	tagName: 'plot-axis-tick'
	parent: Axis
}

/** Title element of an axis element. */
export interface AxisTitle extends TextElement {
	tagName: 'plot-axis-title'
	parent: Axis
}

/** Interlacing element of an axis element. */
export interface AxisInterlacing extends Element {
	tagName: 'plot-axis-interlacing'
	parent: Axis
}

/** Title element of a legend element. */
export interface LegendTitle extends TextElement {
	tagName: 'legend-title'
	parent: Legend
}

/** Marker element of a legend element. */
export interface LegendMarker extends Element {
	tagName: 'legend-marker'
	parent: Legend
	categories?: Data.Record
}

/** Label element of a legend element. */
export interface LegendLabel extends TextElement {
	tagName: 'legend-label'
	parent: Legend
	categories?: Data.Record
}

/** Bar element of a legend element indicating a continous interval. */
export interface LegendBar extends Element {
	tagName: 'legend-bar'
	parent: Legend
}

/** The interface of the event object is passed to event handlers by the library.
    Detail properties will vary by event type. */
export interface Event<T> {
	type: EventType
	/** The element of the chart the event triggered on. */
	target: T | null
	/** If called, the default action of the event will be canceled. */
	preventDefault(): void
}

export interface PointerDetail {
	pointerId: number | null
	position: Point
}

/** Event for pointer/mouse events. */
export interface PointerEvent extends Event<Element> {
	detail: PointerDetail
}

export interface WheelDetail {
	delta: number
}

/** Event for mouse wheel change. */
export interface WheelEvent extends Event<Element> {
	detail: WheelDetail
}

export interface UpdateDetail {
	position: Anim.Duration
	progress: number
}

/** Event emitted at every animation frame update. */
export interface UpdateEvent extends Event<Element> {
	detail: UpdateDetail
}

export interface RectDrawDetail {
	rect: Rect
	relative: boolean
}

/** Event of drawing a rectangle or a marker 
    (in which case rect is the boundary rectangle). */
export interface RectDrawEvent<T> extends Event<T> {
	detail: RectDrawDetail
}

export interface LineDrawDetail {
	line: Line
	relative: boolean
}

/** Event of drawing a line. */
export interface LineDrawEvent<T> extends Event<T> {
	detail: LineDrawDetail
}

/** Event of drawing a text. */
export interface TextDrawDetail {
	/** The transformed rectangle of the text with padding. */
	outerRect: TransformedRect
	/** The rectangle of the text without padding, relative to outerRect. */
	innerRect: Rect
	/** The horizontal alignment of the text which can take a value between -1 (left align) and 1 (right align). */
	align: number
	text: string
}

/** Event of drawing a text. */
export interface TextDrawEvent<T> extends Event<T> {
	detail: TextDrawDetail
}
export type EventHandler<Event> = (event: Event) => void

type EventRecord<EventHandler> = [CFunction | null, EventHandler[]]

interface EventState {
	canceled: boolean
}

type EventHandlers<T extends EventType> = {
	[key in T]?: EventRecord<EventHandler<EventMap[T]>>
}

export class Events {
	private _cChart: CChart
	private _eventHandlers: EventHandlers<EventType> = {}

	constructor(cChart: CChart) {
		this._cChart = cChart
	}

	add<T extends EventType>(eventName: T, handler: EventHandler<EventMap[T]>): void {
		if (typeof eventName !== 'string') {
			throw new Error('first parameter should be string')
		}
		this._getHandlers(eventName).push(handler)
	}

	private _getHandlers<T extends EventType>(eventName: T): EventHandler<EventMap[T]>[] {
		if (!(eventName in this._eventHandlers)) {
			let cfunc = null
			if (!this._isJSEvent(eventName)) {
				const func = (eventPtr: CEvent, param: EventMap[T]): void => {
					this._invoke(eventName, param, eventPtr)
				}
				cfunc = this._cChart.addEventListener(eventName, func)
			}
			this._eventHandlers[eventName] = [cfunc, []]
		}
		const handlers = this._eventHandlers[eventName]
		return handlers![1]
	}

	remove<T extends EventType>(eventName: T, handler: EventHandler<EventMap[T]>): void {
		if (typeof eventName !== 'string') {
			throw new Error('first parameter should be string')
		}

		const eventRecord = this._eventHandlers[eventName]
		if (!eventRecord) throw new Error('unknown event handler')

		const [cfunc, handlers] = eventRecord

		handlers.find((o, i) => {
			if (o === handler) {
				handlers.splice(i, 1)
				return true
			}
			return false
		})

		if (handlers.length === 0) {
			if (!this._isJSEvent(eventName)) {
				if (cfunc) this._cChart.removeEventListener(eventName, cfunc)
			}
			delete this._eventHandlers[eventName]
		}
	}

	addMany(events: PluginListeners): void {
		for (const [eventName, handler] of Object.entries(events)) {
			this.add(eventName as EventType, handler as EventHandler<EventMap[EventType]>)
		}
	}

	removeMany(events: PluginListeners): void {
		for (const [eventName, handler] of Object.entries(events)) {
			this.remove(eventName as EventType, handler as EventHandler<EventMap[EventType]>)
		}
	}

	private _invoke<T extends EventType>(
		eventName: T,
		param: EventMap[T],
		cEvent?: CEvent
	): boolean {
		const state: EventState = { canceled: false }
		try {
			const handlers = this._eventHandlers[eventName]
			if (handlers) {
				for (const handler of [...handlers[1]]) {
					const eventParam = this._isJSEvent(eventName)
						? this._makeJSEventParam(param, state)
						: cEvent
							? this._makeCEventParam(cEvent, param, state)
							: param
					handler(eventParam)
				}
			}
		} catch (e) {
			console.log('exception in event handler: ' + e)
		}
		return state.canceled
	}

	private _isJSEvent(eventName: EventType): boolean {
		return eventName.startsWith('api-')
	}

	private _makeJSEventParam<T>(param: Event<T>, state: EventState): Event<T> {
		param.preventDefault = (): void => {
			state.canceled = true
		}
		return param
	}

	private _makeCEventParam<T>(cEvent: CEvent, param: Event<T>, state: EventState): Event<T> {
		param.preventDefault = (): void => {
			cEvent.preventDefault()
			state.canceled = true
		}
		return param
	}
}
