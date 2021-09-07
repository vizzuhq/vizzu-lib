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
                    y: { set: ['Value 5 (+/-)'], range: {min:'-10%', max:'110%' } },
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
                    x: { set: ['Joy factors', 'Value 6 (+/-)'], range: {min:'0%', max:'110%' } },
                    label: { set: ['Country_code'] }
                },
                title: 'Relationship'
            }
        }
    )
];

export default testSteps;