import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles =
{
    plot: { marker: { borderWidth: 0.5, borderOpacity: 0.7, } }
};

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
                title: 'Treemap (Stacked)',
                legend: 'color'
            },
            style: styles
        }
    )
];

export default testSteps;