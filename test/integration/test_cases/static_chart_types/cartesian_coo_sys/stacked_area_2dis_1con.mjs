import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: ['Country', 'Value 2 (+)'],
                    x: 'Year',
                    color: 'Country',
                },
                title: 'Stacked Area Chart',
                geometry: 'area'
            }
        }
    )
];

export default testSteps;