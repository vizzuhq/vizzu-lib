import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    color: { attach: ['Joy factors'] },
                    size: { attach: ['Value 2 (+)'] },
                    label: { attach: ['Country_code'] }
                },
                title: 'Comparison',
                geometry: 'circle'
            }
        }
    ),    chart => chart.animate(
        {
            config:
            {
                channels:
                {
                    size: { attach: ['Country_code'] }
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
            config: {
                channels:
                {
                    x: { attach: ['Joy factors'] },
                    y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
                    noop: { attach: ['Country_code'] },

                    size: { detach: ['Country_code'] },
                    label: { detach: ['Country_code'] }
                },
                title: 'Distribution'
            },
            style:
            {
                plot: 
                {
                    marker: 
                    {
                        circleMaxRadius: 0.04,
                        fillOpacity:0.8,
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
                    x: { attach: ['Value 6 (+/-)']/*, range: '-20000000,10000000,1'*/ },
                    label: { attach: ['Country_code'] }

                },
                title: 'Relationship'
            },
            style:
            {
                'plot.marker': 
                {
                circleMinRadius: 0.02,
                circleMaxRadius: 0.04
                }
            }
        }
    ),
        chart => chart.animate(
            {
                config: {
                    channels:
                    {
                    x: { detach: ['Joy factors', 'Value 6 (+/-)'] },
                    y: { detach: ['Value 5 (+/-)'] },
                    
                    size: { attach: ['Country_code'] }
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
            config: {
                channels:
                {
                size: { detach: ['Country_code'] }
                },
                title: 'Comparison'
            }
        }
)
];

export default testSteps;