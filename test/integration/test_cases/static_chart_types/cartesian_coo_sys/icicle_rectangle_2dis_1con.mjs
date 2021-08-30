import { data } from '/test/integration/test_data/icicle.js';

const testSteps = [
    chart => chart.animate(
		{
			data: data,
			descriptor : {
				channels: {
					x: { attach: [ 'Joy factors/Country', 'Value 2 (+)'], range: '0,1,%'},
					y: { attach: [ 'Type', 'index' ], title: 'Editable title', range: '0,1,%'},
					label: { attach: [ 'Joy factors/Country' ] },
					color: { attach: [ 'Joy factors/Country' ] }
						},
				title: 'Icicle chart (a hierarchikus adatok szétszedése, még hack!)'
			},
            style:
            {
                plot:
                { marker:
                    { label:
                        {                      
                            orientation: 'vertical',
							angle: 3.14*-1  
                        }
                    }
                }
            }
		}
    )
];

export default testSteps;