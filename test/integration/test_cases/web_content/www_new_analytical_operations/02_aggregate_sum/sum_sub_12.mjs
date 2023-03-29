import { data_8 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- add the Dimension whitch is on the Color channel to the Y-axis too
- switch the Geometry from Circle to Area`
;

const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: 'Year',
                y: 'Value 2 (+)',
                color: 'Country'
            },
            title:'Title',
            geometry: 'circle'
        }
    }
),

     chart => chart.animate({
        config: {
            channels: {
                y: ['Country', 'Value 2 (+)']
            },
            title:'Title',
            geometry: 'area'
        }
     }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;