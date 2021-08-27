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
                    size: { attach: ['Positive nums', 'Country_code'] },
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
                    x: { attach: ['Threat'] },
                    y: { attach: ['Positive nums', 'Country_code'] },
                    size: { detach: ['Positive nums'] },
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
                    x: { detach: ['index', 'Threat'] },
                    y: { detach: ['Positive nums', 'Country_code'] },
                    label: { attach: ['Country_code'] },
                    size: { attach: ['Positive nums'] }
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