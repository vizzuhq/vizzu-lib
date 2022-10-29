import { data } from '../../../test_data/icicle2.mjs';

const testSteps = [
    chart => chart.animate(
		{
			data: data,
			config : {
				channels: {
					x: { attach: ['Level names', 'Value'], range: { max: '100%' }},
					y: { attach: 'Levels', title: 'Levels', range: { max: '100%' }},
					label: { attach:  'Level names'  },
                    lightness: { attach:  'Level names'  },
					color: { attach:  'Level names'  }
						},
				title: 'Fake Icicle Chart'
			},
            style:
            {
                plot: {
                    paddingLeft: '4.14em',
                yAxis: {
                    ticks: { color: '#00000000'},
                    label: { color: '#00000000'},
                    color: '#00000000'
                    },
                xAxis: {
                    label: { color: '#00000000' },
                    interlacing: { color: '#00000000' }
                    },
                marker: {
                    rectangleSpacing:'0',
                    label:
                        {     
                            fontSize: '0.8em',                 
                            orientation: 'vertical',
							angle: 3.14*-1  
                        }
                    }
                }
            }
		}
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;