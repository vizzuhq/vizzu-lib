import { data_6 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- add the Dimension whitch is on Color channel to the Y-axis too
- switch the Geometry from Line to Area`
;

const testSteps = [
    chart => chart.animate({
        data: data_6,

        config: {
            channels: {
                x: 'Year',
                y: 'Value 2 (+)',
                color: 'Country'
            },
            title:'Title',
            geometry: 'line'
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