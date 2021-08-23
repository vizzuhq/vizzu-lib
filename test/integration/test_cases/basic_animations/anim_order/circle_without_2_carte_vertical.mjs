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
                    x: { attach: ['Threat'] },
                    y: { attach: ['Positive nums'] },
                    size: { detach: ['Positive nums'] },
                    label: { detach: ['Threat'] }
                },
                title: 'Y C, X D (X first) - Dotplot'
            }
        }
    ),
        chart => chart.animate(
            {
                descriptor: {
                    channels:
                    {
                        x: { detach: ['Threat'] },
                        y: { detach: ['Positive nums'] },
                        size: { attach: ['Positive nums'] },
                        label: { attach: ['Threat'] }
                    },
                    title: 'Y C, X D (Y first) - Bubble'
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
                title: 'Y C+D, X D - Bubble.'
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
                    x: { attach: ['Threat'] },
                    y: { attach: ['Positive nums', 'Country_code'] },
                    size: { detach: ['Positive nums'] }
                },
                title: 'Y C+D, X D (X first) - Dotplot'
            },
            style: {
                plot: {
                    marker:
                    {
                        label:
                        {
                            fontWeight: 'bold',
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
                    x: { detach: ['Threat'] },
                    y: { detach: ['Positive nums', 'Country_code'] },
                    size: { attach: ['Positive nums', 'Country_code'] },
                },
                title: 'Y C+D, X D (Y first) - Bubble'
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
                    x: { attach: ['Threat'] },
                    y: { attach: ['Positive nums', 'Country_code'] },
                    size: { detach: ['Positive nums'] }
                },
                title: 'Y C+D, X D (X first) - Dotplot'
            },
            style: {
                plot: {
                    marker:
                    {
                        label:
                        {
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
                    x: { attach: ['Pos_small'] }
                },
                title: 'Y C+D, X D+C - Mekko'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { detach: ['Threat','Pos_small'] },
                    y: { detach: ['Positive nums', 'Country_code'] },
                    size: { attach: ['Positive nums', 'Country_code'] },
                },
                title: 'Y C+D, X D+C (Y first) - Bubble'
            },
            style: {
                plot: {
                    marker:
                    {
                        label:
                        {
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
                    label: { detach: ['Country_code'], attach:['Threat']  },
                    size: { detach: ['Country_code'] },
                    lightness: { detach: ['Positive nums'] }
                },
                title: 'Y C, X D+C - Bubble'
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
                    x: { attach: ['Threat','Pos_small'] },
                    y: { attach: ['Positive nums'] },
                    size: { detach: ['Positive nums'] },
                },
                title: 'Y C, X D+C (X first) - Mekko'
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
                    x: { detach: ['Pos_small'] },
                    label: { detach: ['Threat'] }
                },
                title: 'Y C, X D (X first) - Mekko'
            }
        }
    )
];

export default testSteps;