import { data_4 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_4,

        config: {
            channels: {
                x: 'Year',
                y: ['Country', 'Value 2 (+)'],
                color: 'Country'
            },
            title:'Title',
        }
     }
),

     chart => chart.animate({
         config: {
            channels: {
                x: 'Value 5 (+/-)',
                y: 'Value 2 (+)',
                label: 'Year'
            },
            title:'Title',
            geometry: 'circle'
        }
     }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;