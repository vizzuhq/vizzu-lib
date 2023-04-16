import { data_8 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- set the Split parameter to true`
;

const testSteps = [
    
    chart => {
        let f = data_8.filter;
        return chart.animate({
        data: Object.assign(data_8, { filter: record => f(record) && record.Year >= 15 }),
        config: {
            channels: {
                x: ['Country', 'Value 2 (+)'],
                y: {
                    set: 'Year',
                    range: { min: '-3' }
                },
                color: 'Country'
            },
            title: 'Title',
            coordSystem: 'polar'
            }
        }
    )},

    chart => chart.animate({
        config: {
            title: 'Title',
            split: true
        }
    })
];

export default testSteps;