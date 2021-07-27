import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
    {
        data: data,
        descriptor: {
            channels:
            {
                color: { attach: ['Threat'] },
                size: { attach: ['Positive nums'] },
                label: { attach: ['Country_code'] }
            },
            title: 'Without 2 Cartesian - Bubble 2 Scatter.',
            geometry: 'circle',
            legend: 'color',
        }
    }),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { attach: ['Positive nums'] },
                    y: { attach: ['Negative nums'] },
                    size: { detach: ['Positive nums'] },
                    size: { attach: ['Pos_small'] }
                }
            },
            style: {
                plot: {
                    paddingLeft: 150,
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
                    x: { detach: ['Positive nums'] },
                    y: { detach: ['Negative nums'] },
                    size: { attach: ['Positive nums', 'Country_code'] }
                }
            },
            style: {
                plot: {
                    marker: {
                        label: { fontSize: 10 }
                    }
                }
            }
        }
    )
];

export default testSteps;