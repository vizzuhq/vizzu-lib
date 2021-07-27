import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles =
{
    data: { barMaxPadding: 0.13 },
    plot: { marker: { borderWidth: 0 } }
};

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Year'] },
                    x: { attach: ['Country', 'Positive nums'], range: '0,1.35,%' },
                    color: { attach: ['Country'] },
                    lightness: { attach: ['Year'] }
                },
                title: 'Stacked Radial Chart',
                legend: 'color',
                coordSystem: 'polar'
            },
            style: styles
        }
    )
];

export default testSteps;