import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: {
                channels:
                {
                    color: { set: ['Joy factors'] },
                    x: { set: ['Joy factors'] },
                    y: { set: ['Value 5 (+/-)'], range: '-0.1,1.1,%' },
                    noop: { set: ['Country_code'] },
                    label: { set: null }
                },
                title: 'Distribution',
                geometry: 'circle'
            },
            style:
            {
                plot: 
                {
                    marker: 
                    {
                        label: { fontSize: 9 }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            config: {
                channels:
                {
                    x: { set: ['Joy factors', 'Value 6 (+/-)'], range: '-0.1,1.1,%' },
                    label: { set: ['Country_code'] }
                },
                title: 'Relationship'
            }
        }
    ),    
    chart => chart.animate(
        {
            config: {
                channels:
                {
                    x: { set: ['Joy factors'] },
                    label: { set: null }
                },
                title: 'Distribution'
            }
        }
    ),
];

export default testSteps;