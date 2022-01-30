import { data } from '../../../test_data/sunburst.mjs';

const testSteps = [
    chart => chart.animate(
		{
			data: data,
			config: {
				channels: {
					x: { attach: ['Joy factors/Country', 'Value 2 (+)'], range: { min: '0%', max: '100%' } },
					y: { attach: ['Type', 'index'], range: { min: '0%', max: '100%' } },
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
                    },                 
                    yAxis:
                      {
                          title:
                          {
                              color: '#ffffff00'
                          },
                          label:
                          {
                              color: '#ffffff00'
                          }
                      }
                }
            }
		}
    )
];

export default testSteps;