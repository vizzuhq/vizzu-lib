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
                    y: { attach: ['Value 2 (+)', 'Country_code'] },
                    color: { attach: ['Country_code'] },
                },
                title: 'Rectangle - Cartesian 2 Polar'
            },
            style: {
                plot: {
                    marker: {
                        borderOpacity: 1
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




