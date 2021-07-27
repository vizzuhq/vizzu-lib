import { data } from '/test/integration/test_data/icicle.js';

const testSteps = [
    chart => chart.animate(
		{
			data: data,
			descriptor : {
				channels: {
					x: { attach: [ 'Threat/Country', 'Positive nums'], range: '0,1,%'},
					y: { attach: [ 'Type', 'index' ], title: 'Próba (edited title)', range: '0,1,%'},
					label: { attach: [ 'Threat/Country' ] },
					color: { attach: [ 'Threat/Country' ] }
						},
				title: 'Icicle chart (a hierarchikus adatok szétszedése, még hack!)',
				legend: null
			},
		}
    )
];

export default testSteps;