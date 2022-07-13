import { data } from '../../../test_data/sunburst2.mjs';

const testSteps = [
    chart => chart.animate(
		{
			data: data,
			config : {
				channels: {
					x: { attach: ['Level names', 'Value'], range: { max: '100%' }},
					y: { attach: 'Levels', title: 'Levels', range: { max: '100%' }},
					label: { attach:  'Level names'  },
					color: { attach:  'Level names'  }
						},
				title: 'Fake Sunburst & Multi-level Pie Chart',
                coordSystem: 'polar'
			},
            style:
            {
                plot: {
                    paddingLeft: '3.8em',
                yAxis: {
                    ticks: { color: '#00000000'},
                    label: { color: '#00000000'},
                    color: '#00000000'
                    },
                xAxis: {
                    title: { color: '#00000000' },
                    label: { color: '#00000000' },
                    interlacing: { color: '#00000000' }
                    },
                marker: {
                    rectangleSpacing:'0',
                    label:
                        {     
                            fontSize: '0.6em',                 
                            orientation: 'tangential',
							angle: 3.14*-0.5  
                        }
                    }
                }
            }
		}
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;