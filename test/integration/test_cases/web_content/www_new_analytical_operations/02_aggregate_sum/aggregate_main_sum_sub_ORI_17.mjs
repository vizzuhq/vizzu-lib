import { data_6 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- remove the Dimension from the X-axis and Color channel
- shift the Measure from X to the Y-axis (change orientation), and add it to the Label channel
- switch the Geometry from Area to Rectangle`
;

const testSteps = [
    chart => chart.animate({
        data: data_6,

        config: {
            channels: {
                x: 'Year',
                y: ['Value 2 (+)', 'Country'],
                color: 'Country'
            },
            title:'Title',
            geometry: 'area'
        }
    }
),

    chart => chart.animate({
        config: {
            channels: {
                x: 'Value 2 (+)',
                y: 'Country',
                color: null,
                label: 'Value 2 (+)'
            },
            title:'Title',
            geometry: 'rectangle'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;