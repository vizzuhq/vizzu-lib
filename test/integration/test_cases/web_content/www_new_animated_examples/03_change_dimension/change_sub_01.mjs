import { data_6 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_6,

        config: {
            channels: {
                x: 'Year',
                y: ['Country', 'Value 2 (+)'],
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
                y: ['Joy factors', 'Value 3 (+)'],
                color: 'Joy factors'
            },
            title:'Title'
        },
        style: {
            plot: {
                marker: {
                    colorPalette: '#ef675aFF #6d8cccFF #e6cf99FF #9c50abFF'
                }
            }
        }
    }
)
];

export default testSteps;