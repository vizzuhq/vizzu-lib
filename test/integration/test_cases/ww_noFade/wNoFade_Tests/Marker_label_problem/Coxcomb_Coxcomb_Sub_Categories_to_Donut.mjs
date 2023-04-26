import { data_4 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- remove that Dimension from the X-axis, whitch is not on the Color channel
- move the Measure from Y-axis to the X-axis
- add the Measure to the Label channel too`
;

const testSteps = [
    chart => chart.animate({
        data: data_4,

        config: {
            channels: {
                x: ['Country', 'Year'],
                y: { set: 'Value 2 (+)', range: { min: '-200%' } },
                color: 'Country'
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