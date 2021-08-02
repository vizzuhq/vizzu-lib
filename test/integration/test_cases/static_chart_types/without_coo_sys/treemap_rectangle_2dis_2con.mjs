import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles =
{
    plot: { marker: { borderWidth: 0.5, borderOpacity: 0.8, } }
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
                    label: { attach: ['Country_code'] },
                    lightness: { attach: ['Positive nums'] }
                },
                title: 'Treemap (Stacked+2conti)',
                legend: 'lightness'
            },
            style: styles
        }
    )
];

export default testSteps;