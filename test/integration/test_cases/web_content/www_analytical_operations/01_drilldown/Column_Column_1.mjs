import { data_4 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- add 2 new Dimensions to the X-axis 
- and one of them to the Color channel too`
;

const testSteps = [
    chart => chart.animate({
        data: data_4,

        config: {
             channels: {
                y: 'Value 2 (+)',
                label: 'Value 2 (+)'
             },
            title:'Title'
        }
    }
),
         
    chart => chart.animate({
        config: {
            channels: {
                x: ['Year', 'Country'],
                y: 'Value 2 (+)',
                color: 'Country',
                label: null
            },
            title:'Title'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;