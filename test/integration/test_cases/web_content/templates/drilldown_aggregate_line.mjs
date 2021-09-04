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
                    y: { set: ['Value 3 (+)'], range: '0,6000000000,1' },
                    size: { set: ['Country_code'] }
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
                    y: { range: '0,1,%' },
                },
                title: 'Aggregate'
            }
        }
    ),
    chart => chart.animate(
        {
            config:
            {
                channels:
                {
                    y: { range: '0,6000000000,1' },
                },
                title: 'Aggregate'
            }
        }
    ),
    chart => chart.animate(
        {
            config:
            {
                channels:
                {
                    y: { detach: ['Country'] },
                    color: { set: null },
                    size: { set: 'Country' }
                },
                title: 'Aggregate'
            }
        }
    )
];

export default testSteps;