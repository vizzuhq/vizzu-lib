import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    color: { attach: ['Threat'] },
                    size: { attach: ['Positive nums'] }
                },
                title: '1D, 1C - Treemap'
            },
            style: {
                plot: {
                    paddingLeft: -50,
                    marker: {
                        borderWidth: 1,
                        borderOpacity: 0.1,
                        borderOpacityMode: 'straight'
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
                    y: { attach: ['Threat'] },
                    x: { attach: ['Positive nums'], range: '0,1.1,%' },
                    size: { detach: ['Positive nums'] }
                },
                title: 'X C, Y D (Y first) - Column'
            },
            style: {
                plot: {
                    marker: {
                        label: { fontSize: 6 }
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
                        y: { detach: ['Threat'] },
                        x: { detach: ['Positive nums'] },
                        size: { attach: ['Positive nums'] }
                    },
                    title: 'X C, Y D (X first) - Treemap'
                },
                style: {
                    plot: {
                        marker: {
                            label: { fontSize: 6 }
                        }
                    }
                }
            },
        {
            'y':
             {delay: 1.5}
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    lightness: { attach: ['Positive nums'] },
                    label: { attach: ['Country_code'] },
                    size: { attach: ['Positive nums', 'Country_code'] }
                },
                title: 'X C+D, Y D - Treemap.'
            },
            style: {
                plot: {
                    marker: {
                        label: { fontSize: 8 }
                    }
                }
            }
        },
        {
            'x':
             {delay: 1.5}
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    y: { attach: ['Threat'] },
                    x: { attach: ['Positive nums', 'Country_code'], range: '0,1.1,%' },
                    size: { detach: ['Positive nums'] }
                },
                title: 'X C+D, Y D (Y first) - Column'
            }
        },
        {
            'x':
             {delay: 1.5}
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    y: { detach: ['Threat'] },
                    x: { detach: ['Positive nums', 'Country_code'] },
                    size: { attach: ['Positive nums', 'Country_code'] },
                },
                title: 'X C+D, Y D (X first) - Treemap'
            }
        },
        {
            'y':
             {delay: 1.5}
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    y: { attach: ['Threat'] },
                    x: { attach: ['Positive nums', 'Country_code'], range: '0,1.1,%' },
                    size: { detach: ['Positive nums'] }
                },
                title: 'X C+D, Y D (Y first) - Column'
            }
        },
        {
            'x':
             {delay: 1.5}
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    y: { attach: ['Pos_small'] }
                },
                title: 'X C+D, Y D+C - Mekko'
            }
        },
        {
            'x':
             {delay: 1.5}
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    y: { detach: ['Threat','Pos_small'] },
                    x: { detach: ['Positive nums', 'Country_code'] },
                    size: { attach: ['Positive nums', 'Country_code'] },
                },
                title: 'X C+D, Y D+C (X first) - Treemap'
            }
        },
        {
            'y':
             {delay: 1.5}
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    label: { detach: ['Country_code'] },
                    size: { detach: ['Country_code'] },
                    lightness: { detach: ['Positive nums'] }
                },
                title: 'X C+D, Y C - Treemap'
            }
        },
        {
            'y':
             {delay: 1.5}
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    y: { attach: ['Threat','Pos_small'] },
                    x: { attach: ['Positive nums'], range: '0,1.1,%' },
                    size: { detach: ['Positive nums'] },
                },
                title: 'X D+C, Y C (Y first) - Mekko'
            }
        },
        {
            'x':
             {delay: 1.5}
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    y: { detach: ['Pos_small'] }
                },
                title: 'X C, Y C (Y first) - Mekko'
            }
        },
        {
            'x':
             {delay: 1.5}
        }
    )
];

export default testSteps;