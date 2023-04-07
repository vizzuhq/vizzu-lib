import { data_8 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- replace X-axis Measure with Noop channel Dimension
- move Color channel Dimension to Y-axis
- add the other Measure to the Label channel
- set the Split parameter to true
- switch the Geometry from Circle to Rectangle`
;

const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: 'Value 5 (+/-)',
                y: 'Value 2 (+)',
                color: 'Country',
                noop: 'Year'
            },
            title:'Title',
            geometry: 'circle'
        }
    }
),
    chart => chart.animate({
        config: {
            channels: {
                x: 'Year',
                y: ['Country', 'Value 2 (+)'],
                noop: null,
                label: 'Value 2 (+)'
            },
            title:'Title',
            geometry: 'rectangle',
            split: true
        },
        style: {
            plot: {
                marker:{
                    label: {
                        position:'top',
                        fontSize: '0.6em'
                    }
                }
            }
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;