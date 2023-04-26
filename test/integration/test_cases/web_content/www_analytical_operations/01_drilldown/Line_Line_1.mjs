import { data_6 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- add a new Dimension to the Color channel`
;

const testSteps = [
    chart => chart.animate({
        data: data_6,

        config: {
            channels: {
                x: 'Year',
                y: 'Value 2 (+)'
            },
            title:'Title',
            geometry: 'line'
        }
    }
),
    
    chart => chart.animate({
        config: {
            channels: {
                color: 'Country'
            },
            title:'Title'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;