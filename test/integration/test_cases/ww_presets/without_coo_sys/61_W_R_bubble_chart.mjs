import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    color: { attach: 'Country_code' },
                    label: { attach: 'Country_code' },
                    size: { attach: ['Value 5 (+/-)'] }
                },
                title: 'Bubble Chart',
                geometry: 'circle'
            },
            style: {
                plot: {
                    paddingLeft: '0em',
                    paddingRight: '12.42em',
                    },
                legend:{ maxWidth: '20%' },
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;