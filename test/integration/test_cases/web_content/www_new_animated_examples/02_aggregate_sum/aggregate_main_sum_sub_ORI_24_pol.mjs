import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: ['Joy factors', 'Value 6 (+/-)'],
                y: 'Value 5 (+/-)',
                color: 'Joy factors',
                size: 'Value 2 (+)',
                label: 'Country_code'
            },
            title:'Title',
            geometry: 'circle'
        }
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: ['Country_code', 'Value 2 (+)'],
                y: { set: 'Joy factors', range: { min: '-30%' } }, 
                label: null
            },
            title:'Title',
            geometry: 'rectangle',
            orientation: 'vertical',
            coordSystem: 'polar'
        }
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: 'Value 2 (+)',
                label: 'Value 2 (+)'
            }
        }
    })
];

export default testSteps;