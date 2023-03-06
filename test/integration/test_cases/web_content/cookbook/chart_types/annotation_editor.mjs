import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
	async chart => {
		await import('https://cdn.jsdelivr.net/npm/markerjs2@2.29.0/markerjs2.min.js');

		let markerArea = new markerjs2.MarkerArea(chart._container);

		markerArea.show();

		markerArea.addEventListener('render', event => {
			console.log(event.state);
		});

		return chart.animate({
			data: data,
			config: {
				x: 'Joy factors',
				y: 'Value 2 (+)',
				color: 'Joy factors',
				label: 'Value 2 (+)',
				title: 'Chart with Annotations'
			}
		});
	}
];

export default testSteps;