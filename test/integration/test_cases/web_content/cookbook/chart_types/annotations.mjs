import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
	async chart => {
		await import('https://cdn.jsdelivr.net/npm/markerjs-live@1.1.0/markerjs-live.min.js');

		let markerView = new mjslive.MarkerView(chart._container);

		// This object generated using annotation-editor example
		let config = {
			width: chart._container.width,
			height: chart._container.height,
			markers: [
				{
					"arrowType": "end",
					"strokeColor": "#2563EB",
					"strokeWidth": 3,
					"strokeDasharray": "",
					"x1": 458,
					"y1": 221,
					"x2": 422,
					"y2": 316,
					"typeName": "ArrowMarker",
					"state": "select"
				},
				{
					"color": "#2563EB",
					"fontFamily": "Helvetica, Arial, sans-serif",
					"padding": 5,
					"text": "This bar is annotated",
					"left": 434,
					"top": 182,
					"width": 159,
					"height": 37,
					"rotationAngle": 0,
					"visualTransformMatrix": {
						"a": 1,
						"b": 0,
						"c": 0,
						"d": 1,
						"e": 0,
						"f": 0
					},
					"containerTransformMatrix": {
						"a": 1,
						"b": 0,
						"c": 0,
						"d": 1,
						"e": 0,
						"f": 0
					},
					"typeName": "TextMarker",
					"state": "select"
				},
				{
					"fillColor": "#10B981",
					"strokeColor": "transparent",
					"strokeWidth": 0,
					"strokeDasharray": "",
					"opacity": 0.25,
					"left": 108,
					"top": 99,
					"width": 48,
					"height": 26,
					"rotationAngle": 0,
					"visualTransformMatrix": {
						"a": 1,
						"b": 0,
						"c": 0,
						"d": 1,
						"e": 0,
						"f": 0
					},
					"containerTransformMatrix": {
						"a": 1,
						"b": 0,
						"c": 0,
						"d": 1,
						"e": 0,
						"f": 0
					},
					"typeName": "HighlightMarker",
					"state": "select"
				},
				{
					"color": "#2563EB",
					"fontFamily": "Helvetica, Arial, sans-serif",
					"padding": 5,
					"text": "This label is highlighted",
					"left": 153,
					"top": 95,
					"width": 164,
					"height": 30,
					"rotationAngle": 0,
					"visualTransformMatrix": {
						"a": 1,
						"b": 0,
						"c": 0,
						"d": 1,
						"e": 0,
						"f": 0
					},
					"containerTransformMatrix": {
						"a": 1,
						"b": 0,
						"c": 0,
						"d": 1,
						"e": 0,
						"f": 0
					},
					"typeName": "TextMarker",
					"state": "select"
				},
				{
					"bgColor": "#000000",
					"tipPosition": {
						"x": 22.1,
						"y": 91
					},
					"color": "#FFFFFF",
					"fontFamily": "Helvetica, Arial, sans-serif",
					"padding": 5,
					"text": "E!",
					"left": 221,
					"top": 170,
					"width": 48,
					"height": 45,
					"rotationAngle": 0,
					"visualTransformMatrix": {
						"a": 1,
						"b": 0,
						"c": 0,
						"d": 1,
						"e": 0,
						"f": 0
					},
					"containerTransformMatrix": {
						"a": 1,
						"b": 0,
						"c": 0,
						"d": 1,
						"e": 0,
						"f": 0
					},
					"typeName": "CalloutMarker",
					"state": "select"
				}
			]
		};

		return chart.animate({
			data: data,
			config: {
				x: 'Joy factors',
				y: 'Value 2 (+)',
				color: 'Joy factors',
				label: 'Value 2 (+)',
				title: 'Chart with Annotations'
			}
		}).then(chart => {
			markerView.show(config);
			return chart;
		});
	}
];

export default testSteps;