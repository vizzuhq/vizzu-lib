/* eslint-disable @typescript-eslint/no-unused-vars, lines-between-class-members, no-use-before-define */

import { Data } from './data'
import { Geom } from './geom'
import { Anim } from './anim'

export namespace Events {
  enum Type {
    click = 'click',
    pointeron = 'pointeron',
    pointermove = 'pointermove',
    pointerdown = 'pointerdown',
    pointerup = 'pointerup',
    wheel = 'wheel',
    update = 'update',
    drawBegin = 'draw-begin',
    drawComplete = 'draw-complete',
    backgroundDraw = 'background-draw',
    titleDraw = 'title-draw',
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

  interface EventMap {
    [Type.click]: PointerEvent
    [Type.pointeron]: PointerEvent
    [Type.pointermove]: PointerEvent
    [Type.pointerdown]: PointerEvent
    [Type.pointerup]: PointerEvent
    [Type.wheel]: WheelEvent
    [Type.update]: UpdateEvent
    [Type.drawBegin]: Event<Element>
    [Type.drawComplete]: Event<Element>
    [Type.backgroundDraw]: RectDrawEvent<Root>
    [Type.titleDraw]: TextDrawEvent<Title>
    [Type.logoDraw]: RectDrawEvent<Logo>
    [Type.legendBackgroundDraw]: RectDrawEvent<Legend>
    [Type.legendTitleDraw]: TextDrawEvent<LegendTitle>
    [Type.legendLabelDraw]: TextDrawEvent<LegendLabel>
    [Type.legendMarkerDraw]: RectDrawEvent<LegendMarker>
    [Type.legendBarDraw]: RectDrawEvent<LegendBar>
    [Type.plotBackgroundDraw]: RectDrawEvent<Plot>
    [Type.plotAreaDraw]: RectDrawEvent<Area>
    [Type.plotMarkerDraw]: RectDrawEvent<Marker>
    [Type.plotMarkerLabelDraw]: TextDrawEvent<MarkerLabel>
    [Type.plotMarkerGuideDraw]: LineDrawEvent<MarkerGuide>
    [Type.plotAxisDraw]: LineDrawEvent<Axis>
    [Type.plotAxisTitleDraw]: TextDrawEvent<AxisTitle>
    [Type.plotAxisLabelDraw]: TextDrawEvent<AxisLabel>
    [Type.plotAxisTickDraw]: LineDrawEvent<AxisTick>
    [Type.plotAxisGuideDraw]: LineDrawEvent<AxisGuide>
    [Type.plotAxisInterlacingDraw]: RectDrawEvent<AxisInterlacing>
    [Type.animationBegin]: Event<Element>
    [Type.animationComplete]: Event<Element>
  }

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
  interface Event<T> {
    type: Type
    /** The element of the chart the event triggered on. */
    target: T | null
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
  interface PointerEvent extends Event<Element> {
    detail: PointerDetail
  }
  interface WheelDetail {
    delta: number
  }
  /** Event for mouse wheel change. */
  interface WheelEvent extends Event<Element> {
    detail: WheelDetail
  }
  interface UpdateDetail {
    position: Anim.Duration
    progress: number
  }
  /** Event emitted at every animation frame update. */
  interface UpdateEvent extends Event<Element> {
    detail: UpdateDetail
  }
  interface RectDrawDetail {
    rect: Geom.Rect
    relative: boolean
  }
  /** Event of drawing a rectangle or a marker 
    (in which case rect is the boundary rectangle). */
  interface RectDrawEvent<T> extends Event<T> {
    detail: RectDrawDetail
  }
  interface LineDrawDetail {
    line: Geom.Line
    relative: boolean
  }
  /** Event of drawing a line. */
  interface LineDrawEvent<T> extends Event<T> {
    detail: LineDrawDetail
  }
  interface TextDrawDetail {
    rect: Geom.TransformedRect
    text: string
  }
  /** Event of drawing a text. */
  interface TextDrawEvent<T> extends Event<T> {
    detail: TextDrawDetail
  }
  type Handler<Event> = (event: Event) => void
}
