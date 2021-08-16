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
                    color: { attach: ['Country_code'] }
                },
                title: 'Area - Cartesian 2 Polar',
                geometry: 'area',
                legend: 'color'
            },
            style:
            {
                plot: {
                    paddingLeft: 70
                },
                fontFamily: 'roboto-regular'
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