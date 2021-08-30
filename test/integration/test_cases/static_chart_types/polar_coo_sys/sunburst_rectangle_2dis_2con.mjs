import { data } from '/test/integration/test_data/sunburst.js';

const testSteps = [
	chart => chart.animate(
		{
			data: data,
			descriptor: {
				channels: {
					x: { attach: ['Joy factors/Country', 'Value 2 (+)'], range: '0,1,%' },
					y: { attach: ['Type', 'index'] },
					label: { attach: ['Joy factors/Country'] },
					color: { attach: ['Joy factors/Country'] },
					lightness: { attach: ['Value 5 (+/-)'] }
				},
				title: 'Sunburst chart (a hierarchikus adatok szétszedése, még hack!)',
				coordSystem: 'polar',
//				legend: 'lightness'
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