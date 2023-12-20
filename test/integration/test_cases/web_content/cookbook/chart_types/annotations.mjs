import { data } from '../../../../test_data/chart_types_eu.mjs'

const xScale = 0.95
const yScale = 0.85

const testSteps = [
	async (chart) => {
		await import('https://cdn.jsdelivr.net/npm/markerjs-live@1.1.0/markerjs-live.min.js')

		const markerView = new mjslive.MarkerView(chart.feature.htmlCanvas.element)

		// This object generated using annotation-editor example
		const config = {
			width: chart.feature.htmlCanvas.element.width,
			height: chart.feature.htmlCanvas.element.height,
			markers: [
				{
					arrowType: 'end',
					strokeColor: '#2563EB',
					strokeWidth: 3,
					strokeDasharray: '',
					x1: 458 * xScale,
					y1: 221 * yScale,
					x2: 422 * xScale,
					y2: 316 * yScale,
					typeName: 'ArrowMarker',
					state: 'select'
				},
				{
					color: '#2563EB',
					fontFamily: 'Helvetica, Arial, sans-serif',
					padding: 5,
					text: 'This bar is annotated',
					left: 434 * xScale,
					top: 182 * yScale,
					width: 159 * xScale,
					height: 37 * yScale,
					rotationAngle: 0,
					visualTransformMatrix: {
						a: 1,
						b: 0,
						c: 0,
						d: 1,
						e: 0,
						f: 0
					},
					containerTransformMatrix: {
						a: 1,
						b: 0,
						c: 0,
						d: 1,
						e: 0,
						f: 0
					},
					typeName: 'TextMarker',
					state: 'select'
				},
				{
					fillColor: '#10B981',
					strokeColor: 'transparent',
					strokeWidth: 0,
					strokeDasharray: '',
					opacity: 0.25,
					left: 108 * xScale,
					top: 99 * yScale,
					width: 48 * xScale,
					height: 26 * yScale,
					rotationAngle: 0,
					visualTransformMatrix: {
						a: 1,
						b: 0,
						c: 0,
						d: 1,
						e: 0,
						f: 0
					},
					containerTransformMatrix: {
						a: 1,
						b: 0,
						c: 0,
						d: 1,
						e: 0,
						f: 0
					},
					typeName: 'HighlightMarker',
					state: 'select'
				},
				{
					color: '#2563EB',
					fontFamily: 'Helvetica, Arial, sans-serif',
					padding: 5,
					text: 'This label is highlighted',
					left: 153 * xScale,
					top: 95 * yScale,
					width: 164 * xScale,
					height: 30 * yScale,
					rotationAngle: 0,
					visualTransformMatrix: {
						a: 1,
						b: 0,
						c: 0,
						d: 1,
						e: 0,
						f: 0
					},
					containerTransformMatrix: {
						a: 1,
						b: 0,
						c: 0,
						d: 1,
						e: 0,
						f: 0
					},
					typeName: 'TextMarker',
					state: 'select'
				},
				{
					bgColor: '#000000',
					tipPosition: {
						x: 22.1,
						y: 91
					},
					color: '#FFFFFF',
					fontFamily: 'Helvetica, Arial, sans-serif',
					padding: 5,
					text: 'E!',
					left: 221 * xScale,
					top: 170 * yScale,
					width: 48 * xScale,
					height: 45 * yScale,
					rotationAngle: 0,
					visualTransformMatrix: {
						a: 1,
						b: 0,
						c: 0,
						d: 1,
						e: 0,
						f: 0
					},
					containerTransformMatrix: {
						a: 1,
						b: 0,
						c: 0,
						d: 1,
						e: 0,
						f: 0
					},
					typeName: 'CalloutMarker',
					state: 'select'
				}
			]
		}

		return chart
			.animate({
				data,
				config: {
					x: 'Joy factors',
					y: 'Value 2 (+)',
					color: 'Joy factors',
					label: 'Value 2 (+)',
					title: 'Chart with Annotations'
				}
			})
			.then((chart) => {
				markerView.show(config)
				return chart
			})
	}
]

export default testSteps
