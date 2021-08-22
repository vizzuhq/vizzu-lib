import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    x: { attach: ['Year'] },
                    y: { attach: ['Positive nums', 'Country_code'] },
                    color: { attach: ['Country_code'] },
                },
                title: 'Rectangle - Cartesian 2 Polar',
                legend: 'color'
            },
            style: {
                plot: {
                    marker: {
                        borderWidth: 0
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                coordSystem: 'polar'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                coordSystem: 'cartesian'
            }
        }
    )
];

export default testSteps;




