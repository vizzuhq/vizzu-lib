import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    color: { attach: ['Country_code'] },
                    size: { attach: ['Value 2 (+)', 'Country_code'] },
                    label: { attach: ['Country_code'] }
                },
                title: 'Treemap',
//                legend:null
            },
            style: {
                legend:{ maxWidth: '20%' },
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;