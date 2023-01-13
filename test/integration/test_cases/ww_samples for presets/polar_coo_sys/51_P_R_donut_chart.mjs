import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: ['Joy factors', 'Value 2 (+)'],
                    y: {range: {min:'-200%', max:'100%'} },
                    color: 'Joy factors',
                    label: 'Value 2 (+)'
                },
                title: 'Donut Chart',
                coordSystem: 'polar'
            }
        }
    )
];

export default testSteps;