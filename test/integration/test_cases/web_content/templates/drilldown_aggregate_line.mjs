import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { set: ['Year'] },
                    y: { set: ['Value 3 (+)'], range: {min:'0', max:'6000000000' } },
                    size: { set: ['Country'] }
                },
                title: 'Line Chart',
                geometry: 'line'
            }
        }
    ),
    chart => chart.animate(
        {    
            config:
            {
                channels: 
                {
                    color: { set: ['Country'] },
                    size: { set: null }                    
                },
                title: 'Drill Down'
            }
        }
    ),
    chart => chart.animate(
        {
            config:
            {
                channels:
                {
                    y: { range: {min:'0%', max:'100%' } },
                },
                title: 'Aggregate'
            }
        }
    )
];

export default testSteps;