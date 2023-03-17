import { data_4 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_4,

        config:  {
            channels:  {
                x: 'Value 2 (+)',
                y: 'Value 3 (+)',
                noop: 'Year',
                color: 'Country'
            },
            title: 'Scatter plot',
            geometry: 'circle',
        }
    }
),

    chart => chart.animate({
        config: {
            channels: {
                y: ['Value 3 (+)','Country']
            },
            title: 'Split Components to See them Separately',
            geometry: 'circle',
            orientation: 'horizontal',
            split: true
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;