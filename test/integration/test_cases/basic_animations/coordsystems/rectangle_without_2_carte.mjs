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
                    x: { attach: ['Threat'] },
                    y: { attach: ['Positive nums', 'Country_code'] },
                    size: { detach: ['Positive nums'] },
                    lightness: { attach: ['Country_code'] },
                },
                legend: 'color',
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
                    x: { attach: ['index'] },
                    label: { detach: ['Country_code'] }
                },
                legend: 'lightness'
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
                },
                legend: 'noop'
            },
            style: {
                plot: {
                    paddingLeft: -50,
                    marker: {
                        label: { fontSize: 10 }
                    }
                }
            }
        }
    )
];

export default testSteps;