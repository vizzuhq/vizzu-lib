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
                    size: { attach: ['Positive nums'] },
                    label: { attach: ['Threat'] }
                },
                title: '1D, 1C - Bubble',
                geometry: 'circle'
            },
            style: {
                plot: {
                    paddingLeft: -50,
                    marker:
                    {
                        label:
                        {
                            fontWeight: 'bold'
                        }
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
                    x: { attach: ['Positive nums'] },
                    size: { detach: ['Positive nums'] },
                    label: { detach: ['Threat'] }
                },
                title: 'X C, Y D (Y first) - Dotplot'
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
                        size: { attach: ['Positive nums'] },
                        label: { attach: ['Threat'] }
                    },
                    title: 'X C, Y D (X first) - Bubble'
                }
            }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    lightness: { attach: ['Positive nums'] },
                    label: { attach: ['Country_code'], detach: ['Threat'] },
                    size: { attach: ['Positive nums', 'Country_code'] }
                },
                title: 'X C+D, Y D - Bubble.'
            },
            style: {
                plot: {
                    marker: {
                        label: { fontSize: 8 }
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
                    x: { attach: ['Positive nums', 'Country_code'] },
                    size: { detach: ['Positive nums'] }
                },
                title: 'X C+D, Y D (Y first) - Dotplot'
            },
            style: {
                plot: {
                    marker:
                    {
                        label:
                        {
                            fontWeight: 'bold',
                            position: 'top',
                            filter: 'lightness(0.2)',
                            numberFormat: 'grouped'
                        }
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
                    x: { detach: ['Positive nums', 'Country_code'] },
                    size: { attach: ['Positive nums', 'Country_code'] },
                },
                title: 'X C+D, Y D (X first) - Bubble'
            },
            style: {
                plot: {
                    marker:
                    {
                        label:
                        {
                            position: null,
                            filter: null,
                            numberFormat: null
                        }
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
                    x: { attach: ['Positive nums', 'Country_code'] },
                    size: { detach: ['Positive nums'] }
                },
                title: 'X C+D, Y D (Y first) - Dotplot'
            },
            style: {
                plot: {
                    marker:
                    {
                        label:
                        {
                            position: 'top',
                            filter: 'lightness(0.2)',
                            numberFormat: 'grouped'
                        }
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
                    y: { attach: ['Pos_small'] }
                },
                title: 'X C+D, Y D+C - Mekko'
            }
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
                title: 'X C+D, Y D+C (X first) - Bubble'
            },
            style: {
                plot: {
                    marker:
                    {
                        label:
                        {
                            position: null,
                            filter: null,
                            numberFormat: null
                        }
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
                    label: { detach: ['Country_code'], attach:['Threat'] },
                    size: { detach: ['Country_code'] },
                    lightness: { detach: ['Positive nums'] }
                },
                title: 'X C+D, Y C - Bubble'
            },
            style: {
                plot: {
                    marker:
                    {
                        label:
                        {
                            fontSize: '12'
                        }
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
                    y: { attach: ['Threat','Pos_small'] },
                    x: { attach: ['Positive nums'] },
                    size: { detach: ['Positive nums'] }
                },
                title: 'X D+C, Y C (Y first) - Mekko'
            },
            style: {
                plot: {
                    marker:
                    {
                        label:
                        {
                            filter: 'lightness(0.2)',
                        }
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
                    y: { detach: ['Pos_small'] },
                    label: { detach: ['Threat'] }
                },
                title: 'X C, Y C (Y first) - Mekko'
            }
        }
    )
];

export default testSteps;