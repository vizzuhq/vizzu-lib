import { data_8 } from '../../../../test_data/chart_types_eu.mjs';


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
                x: 'Joy factors',
//                label: 'Country'
            },
            title:'Title'
        }
    }
)
];

export default testSteps;