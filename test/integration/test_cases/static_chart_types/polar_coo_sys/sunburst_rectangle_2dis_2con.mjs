import { data } from '/test/integration/test_data/sunburst.js';

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
			style:
			{
				plot: 
				{
					marker: 
					{
						borderWidth: 0,
						label:
                        {   fontSize: 11,
                            orientation: 'normal',
							angle: 3.14*-1,
                            filter: 'lightness(0.6)'  
                        }
					}
				}
			}
		}
	)
];

export default testSteps;