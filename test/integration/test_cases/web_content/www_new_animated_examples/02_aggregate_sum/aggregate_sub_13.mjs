import { data_6 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_6,

        config: {
            channels:  {
                x: 'Value 1 (+)',
                y: 'Value 3 (+)',
                color: 'Country',
                label: 'Year'
            },
            title:'Title',
            geometry: 'circle'
        }
    }
),

    chart => chart.animate({
        config: {
            channels:  {
                label: 'Country'
            },
            title:'Title'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;