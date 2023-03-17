import { data_6 } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data_6,
        config: {
            channels: {
                x: 'Year',
                y: 'Value 3 (+)'
            },
            title:'Title',
            geometry: 'line'
        }
    }
),

    chart => chart.animate({
        config: {
            channels: {
                y: ['Country', 'Value 3 (+)'],
                color: 'Country'
            },
            title:'Title',
            geometry: 'area'
        }
    }
)
];

export default testSteps;