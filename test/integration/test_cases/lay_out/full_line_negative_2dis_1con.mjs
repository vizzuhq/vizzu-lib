import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Value 5 (+/-)'], range: '0,1.1,%'},
                    x: { attach: ['Year']},
                    color: { attach: ['Country']}
                },
                title: 'Line Chart with (-) Nums (2disc)',
                geometry: 'line'
            },
            style: 
            {
                title: {backgroundColor: 'rgb(200,200,200 )'},
                legend:{backgroundColor: 'rgb(230,230,230 )'},
                plot: 
                { 
                    backgroundColor: 'rgb(215,215,215 )',
                    marker: 
                    {
                        borderOpacity:1
                    } 
                },
                data: 
                {
                    columnMaxPadding: 0.21 
                }
            }
        }
    )
];

export default testSteps;