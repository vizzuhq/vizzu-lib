import { data } from '/test/integration/test_data/sunburst.js';

let styles = {
	plot: {
		marker: {
			borderWidth: 0,
		}
	}
};

const testSteps = [
	chart => chart.animate(
		{
			data: data,
			descriptor: {
				channels: {
					x: { attach: ['Threat/Country', 'Positive nums'], range: '0,1,%' },
					y: { attach: ['Type', 'index'] },
					label: { attach: ['Threat/Country'] },
					color: { attach: ['Threat/Country'] },
					lightness: { attach: ['Positive nums'] }
				},
				title: 'Sunburst chart (a hierarchikus adatok szétszedése, még hack!)',
				coordSystem: 'polar',
				legend: 'lightness'
			},
			style: styles
		}
	)
];

export default testSteps;