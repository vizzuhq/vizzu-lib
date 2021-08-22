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
                title: '1D, 1C - Treemap'
            },
            style: {
                plot: {
                    paddingLeft: -50,
                    marker: {
                        label: { fontSize: 14 }
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
                    x: { attach: ['Threat', 'index'] },
                    y: { attach: ['Positive nums'] },
                    size: { detach: ['Positive nums'] },
                    label: { detach: ['Threat'] }
                },
                title: 'Y C, X D (X first) - Coxcomb',
                coordSystem: 'polar'
            }
        }
    ),
        chart => chart.animate(
            {
                descriptor: {
                    channels:
                    {
                        x: { detach: ['Threat', 'index'] },
                        y: { detach: ['Positive nums'] },
                        size: { attach: ['Positive nums'] },
                        label: { attach: ['Threat'] }
                    },
                    title: 'Y C, X D (Y first) - Treemap',
                    coordSystem: 'cartesian'
                }
            },
        {
            'y':
             {delay: 0, duration: 0.75},
             'x':
             {delay: 0.75, duration: 0.75},
             'coordSystem':
             {delay: 0, duration: 0.75}
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    lightness: { attach: ['Positive nums'] },
                    label: { attach: ['Country_code'], detach:['Threat'] },
                    size: { attach: ['Positive nums', 'Country_code'] }
                },
                title: 'Y C+D, X D - Treemap.'
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
            'y':
             {delay: 1.5}
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { attach: ['Threat', 'index'] },
                    y: { attach: ['Positive nums', 'Country_code'] },
                    size: { detach: ['Positive nums'] }
                },
                title: 'Y C+D, X D (X first) - Coxcomb',
                coordSystem: 'polar'
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
                    x: { detach: ['Threat', 'index'] },
                    y: { detach: ['Positive nums', 'Country_code'] },
                    size: { attach: ['Positive nums', 'Country_code'] },
                },
                title: 'Y C+D, X D (Y first) - Treemap',
                coordSystem: 'cartesian'
            }
        },
        {
            'x':
             {delay: 1}
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
                title: 'Y C+D, X D (X first) - Coxcomb',
                coordSystem: 'polar'
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
                    x: { attach: ['Pos_small'] }
                },
                title: 'Y C+D, X D+C - Mekko'
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
                    x: { detach: ['Threat','Pos_small'] },
                    y: { detach: ['Positive nums', 'Country_code'] },
                    size: { attach: ['Positive nums', 'Country_code'] },
                },
                title: 'Y C+D, X D+C (Y first) - Treemap',
                coordSystem: 'cartesian'
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
                    label: { detach: ['Country_code'], attach: ['Threat'] },
                    size: { detach: ['Country_code'] },
                    lightness: { detach: ['Positive nums'] }
                },
                title: 'Y C, X D+C - Treemap'
            },
            style: {
                plot: {
                    marker: {
                        label: { fontSize: '12', position: 'center' }
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
                    x: { attach: ['Threat','Pos_small'] },
                    y: { attach: ['Positive nums'] },
                    size: { detach: ['Positive nums'] },
                    label: { detach: ['Threat'] }
                },
                title: 'Y C, X D+C (X first) - Coxcomb-Mekko',
                coordSystem: 'polar'
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
                    x: { detach: ['Pos_small'] },
                    x: { attach: ['index'] }
                },
                title: 'Y C, X D (X first) - Coxcomb'
            }
        },
        {
            'y':
             {delay: 1.5}
        }
    )
];

export default testSteps;