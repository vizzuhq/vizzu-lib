import { data_4 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_4,

        config: {
            channels: {
                x: ['Country', 'Year'],
                y: 'Value 2 (+)',
                color: 'Country',
                label: null
            },
            title:'Title',
            coordSystem: 'polar'
        }
    }
),
  
    chart => chart.animate({
        config: {
            channels: {
                x: ['Country', 'Value 2 (+)'],
                y: { set: null, range: { min: '-200%' } },
                label: 'Value 2 (+)'
            },
            title:'Title'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;