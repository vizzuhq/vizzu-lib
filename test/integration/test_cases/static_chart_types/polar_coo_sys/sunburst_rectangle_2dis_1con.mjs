import { data } from '/test/integration/test_data/sunburst.js';

const testSteps = [
    chart => chart.animate(
		{
			data: data,
			config: {
				channels: {
					x: { attach: ['Joy factors/Country', 'Value 2 (+)'], range: { min: '0%', max: '1%' } },
					y: { attach: ['Type', 'index'], range: { min: '0%', max: '1%' } },
					label: { attach: ['Joy factors/Country'] },
					color: { attach: ['Joy factors/Country'] }
				},
				title: 'Sunburst chart (a hierarchikus adatok szétszedése, még hack!)',
				coordSystem: 'polar'
			},
            style: 
            {
                plot: {
                    marker: {
                        label:                         
                        {
                            orientation: 'tangential',
                            angle: 3.14*-0.5
                        }
                    }
                }
            }
		}
    )
];

export default testSteps;