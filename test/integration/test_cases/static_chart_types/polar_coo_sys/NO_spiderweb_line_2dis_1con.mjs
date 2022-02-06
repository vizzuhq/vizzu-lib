import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Value 2 (+)'], range: { min: '0%', max: '110%' } },
                    x: { attach: ['Year'] },
                    color: { attach: ['Country'] }
                },
                title: 'Spiderweb (fake. no multiaxes)',
                geometry: 'line',
                coordSystem: 'polar'
            }
        }
    )
];

export default testSteps;