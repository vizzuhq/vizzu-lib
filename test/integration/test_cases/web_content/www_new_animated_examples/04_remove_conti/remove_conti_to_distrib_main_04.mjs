import { data_8 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: 'Value 2 (+)',
                y: 'Value 3 (+)',
                noop: 'Year',
                color: 'Country'
            },
            title: 'Scatter plot',
            geometry: 'circle'
        }
    }
),

    chart => chart.animate({
        config: {
            channels: {
                x: 'Year',
            },
            title: 'If Remove a Measure, then a Dot plot is Better',
            geometry: 'circle',
            orientation: 'horizontal'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;