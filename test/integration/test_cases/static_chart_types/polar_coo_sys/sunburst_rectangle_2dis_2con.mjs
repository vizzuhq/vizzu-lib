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
                plot: {
                    marker: {
                        label:                         
                        {
                            orientation: 'tangential',
                            angle: 3.14*-0.5
                        }
                    },
                    xAxis:
                    { 
                        title:
                        {
							side: 'positive',
                            paddingTop: '25' 
                        }
                    },
                    yAxis:
                    { 
                        title:
                        {
                            paddingBottom: '25' 
                        }
                    }
                }
            }
		}
	)
];

export default testSteps;