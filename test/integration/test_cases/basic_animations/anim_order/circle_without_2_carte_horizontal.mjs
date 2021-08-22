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
            }
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
                title: 'X C+D, Y C - Bubble'
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
            }
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
        }
    )
];

export default testSteps;