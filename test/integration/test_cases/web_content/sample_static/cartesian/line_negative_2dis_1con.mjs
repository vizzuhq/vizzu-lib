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
                title: 'Line Chart',
                geometry: 'line'
            }
        }
    )
];

export default testSteps;