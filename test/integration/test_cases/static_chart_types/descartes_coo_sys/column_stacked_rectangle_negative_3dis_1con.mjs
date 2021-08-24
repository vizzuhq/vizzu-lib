import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles =
{
    data: { columnMaxPadding: 0.12 },
    plot: {
        marker: {
            borderWidth: 0
        }
    }
};

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Negative nums', 'Country_code'] },
                    x: { attach: ['Year', 'Threat'] },
                    color: { attach: ['Country_code'] },
                    size: { attach: ['Country_code', 'Positive nums'] },
                    noop: { attach: ['Year'] }
                },
                title: 'Stacked Column Chart with (-) Nums',
                legend: 'color'
            },
            style: styles
        }
    )
];

export default testSteps;