import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    color: { attach: ['Joy factors'] },
                    size: { attach: ['Value 2 (+)'] },
                    label: { attach: ['Joy factors'] }
                },
                title: '1D, 1C - Bubble',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { attach: ['Joy factors'] },
                    y: { attach: ['Value 2 (+)'] },
                    size: { detach: ['Value 2 (+)'] },
                    label: { detach: ['Joy factors'] }
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
                        x: { detach: ['Joy factors'] },
                        y: { detach: ['Value 2 (+)'] },
                        size: { attach: ['Value 2 (+)'] },
                        label: { attach: ['Joy factors'] }
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
                    lightness: { attach: ['Value 2 (+)'] },
                    label: { attach: ['Country_code'], detach: ['Joy factors'] },
                    size: { attach: ['Value 2 (+)', 'Country_code'] }
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
                    x: { attach: ['Joy factors'] },
                    y: { attach: ['Value 2 (+)', 'Country_code'] },
                    size: { detach: ['Value 2 (+)'] }
                },
                title: 'Y C+D, X D (X first) - Dotplot'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { detach: ['Joy factors'] },
                    y: { detach: ['Value 2 (+)', 'Country_code'] },
                    size: { attach: ['Value 2 (+)', 'Country_code'] },
                },
                title: 'Y C+D, X D (Y first) - Bubble'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { attach: ['Joy factors'] },
                    y: { attach: ['Value 2 (+)', 'Country_code'] },
                    size: { detach: ['Value 2 (+)'] }
                },
                title: 'Y C+D, X D (X first) - Dotplot'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { attach: ['Value 1 (+)'] }
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
                    x: { detach: ['Joy factors','Value 1 (+)'] },
                    y: { detach: ['Value 2 (+)', 'Country_code'] },
                    size: { attach: ['Value 2 (+)', 'Country_code'] },
                },
                title: 'Y C+D, X D+C (Y first) - Bubble'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    label: { detach: ['Country_code'], attach:['Joy factors']  },
                    size: { detach: ['Country_code'] },
                    lightness: { detach: ['Value 2 (+)'] }
                },
                title: 'Y C, X D+C - Bubble'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { attach: ['Joy factors','Value 1 (+)'] },
                    y: { attach: ['Value 2 (+)'] },
                    size: { detach: ['Value 2 (+)'] },
                },
                title: 'Y C, X D+C (X first) - Mekko'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { detach: ['Value 1 (+)'] },
                    label: { detach: ['Joy factors'] }
                },
                title: 'Y C, X D (X first) - Mekko'
            }
        }
    )
];

export default testSteps;