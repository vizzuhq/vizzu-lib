import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    color: { set: ['Joy factors'] },
                    label: { set: ['Country_code'] },
                    size: { set: ['Country_code', 'Value 2 (+)'] }
                },
                title: 'Fragmented',
                geometry: 'circle'
            }
        }
    ),
        chart => chart.animate(
        {
            descriptor:
            {
                channels:
                {
                    x: { set: ['Country_code', 'Value 2 (+)' ], range: '0,1.1,%' },
                    y: { set: ['Joy factors'] },

                    label: { set: null },
                    size: { set: null }
                },
                title: 'Amount',
                geometry: 'rectangle',
                orientation: 'vertical'
            }
        }
    )
];

export default testSteps;