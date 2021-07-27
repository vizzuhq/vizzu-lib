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
					y: { attach: ['Type', 'index'], range: '0,1,%' },
					label: { attach: ['Threat/Country'] },
					color: { attach: ['Threat/Country'] }
				},
				title: 'Sunburst chart (a hierarchikus adatok szétszedése, még hack!)',
				coordSystem: 'polar',
				legend: null
			},
			style: styles
		}
    )
];

export default testSteps;