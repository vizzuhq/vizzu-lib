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
                    size: { attach: ['Value 2 (+)', 'Country_code'] },
                    label: { attach: ['Country_code'] }
                },
                title: 'Without 2 Cartesian - Treemap 2 Column.'
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
                    size: { detach: ['Value 2 (+)'] },
                    lightness: { attach: ['Country_code'] },
                }
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
                    x: { attach: ['index'] },
                    label: { detach: ['Country_code'] }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { detach: ['index', 'Joy factors'] },
                    y: { detach: ['Value 2 (+)', 'Country_code'] },
                    label: { attach: ['Country_code'] },
                    size: { attach: ['Value 2 (+)'] }
                }
            },
            style: {
                plot: {
                    marker: {
                        label: { fontSize: null }
                    }
                }
            }
        }
    )
];

export default testSteps;