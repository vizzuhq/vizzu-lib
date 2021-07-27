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
                    label: { attach: ['Country_code'] },
                },
                title: 'Without 2 polar - Treemap 2 Coxcomb.',
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
                    x: { attach: ['Threat', 'index'] },
                    y: { attach: ['Positive nums', 'Country_code'] },
                    size: { detach: ['Positive nums'] },
                    label: { detach: ['Country_code'] }
                },
                coordSystem: 'polar'
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
                    x: { detach: ['index', 'Threat'] },
                    y: { detach: ['Positive nums', 'Country_code'] },
                    label: { attach: ['Country_code'] },
                    size: { attach: ['Positive nums'] },
                    lightness: { attach: ['Country_code'] }
                },
                coordSystem: 'cartesian',
                legend: 'shape'
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