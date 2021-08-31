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
                title: null,
                geometry: 'line',
                legend: null
            },
            style: 
                {
                legend:
                {
                    backgroundColor: 'rgb(180,180,180 )' 
                }
            }
        }
    )
];

export default testSteps;