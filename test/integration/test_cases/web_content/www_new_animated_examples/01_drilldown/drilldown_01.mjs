import { data_8 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                y: 'Value 2 (+)',
                label: 'Value 2 (+)'
            }
        }
    }
),

    chart => chart.animate({
            config: {
            channels: {
                x: 'Country'
            }
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;