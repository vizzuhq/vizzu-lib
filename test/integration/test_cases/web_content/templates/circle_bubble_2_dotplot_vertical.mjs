import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    color: { set: ['Joy factors'] },
                    size: { set: ['Value 2 (+)'] },
                    label: { set: ['Country_code'] }
                },
                title: 'Comparison',
                geometry: 'circle'
            }
        }
    ),    chart => chart.animate(
        {
            descriptor:
            {
                channels:
                {
                    size: { set: ['Value 2 (+)', 'Country_code'] }
                },
                title: 'Stacked Comparison',
                geometry: 'circle'
            },
            style: 
            {
                plot: 
                {
                    marker: 
                    {
                        label: { fontSize: 10 }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { set: ['Joy factors'] },
                    y: { set: ['Value 5 (+/-)'], range: '0,1.1,%' },
                    noop: { set: ['Country_code'] },
                    size: { set: ['Value 2 (+)'] },
                    label: { set: null }
                },
                title: 'Distribution'
            },
            style:
            {
                plot: 
                {
                    marker: 
                    {
                        fillOpacity:0.8,
                        label: { fontSize: 9 }
                    }
                },
                data: 
                {
                circleMaxRadius: 0.04
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { set: ['Joy factors', 'Value 6 (+/-)']/*, range: '-20000000,10000000,1'*/ },
                    label: { set: ['Country_code'] }
                },
                title: 'Correlation'
            },
            style:
            {
                data: 
                {
                circleMinRadius: 5,
                circleMaxRadius: 0.04
                }
            }
        }
    ),
        chart => chart.animate(
            {
                descriptor: {
                    channels:
                    {
                    x: { set: null },
                    y: { set: null },
                    size: { set: ['Value 2 (+)', 'Country_code'] }
                    },
                    title: 'Stacked Comparison'
                },
                style:
                {
                    plot: 
                    {
                        marker: 
                        {
                            fillOpacity:1
                        }
                    }
                }
            }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                size: { set: ['Value 2 (+)'] }
                },
                title: 'Comparison'
            }
        }
)
];

export default testSteps;