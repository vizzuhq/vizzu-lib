import { data_6 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- replace the Dimension on the X-axis with a new one`
;

const testSteps = [
    chart => {
        let f = data_6.filter;
        return chart.animate({
            data: Object.assign(data_6, { 
                filter: record => f(record) && record.Year >= 10 }),

        config: {
            channels: {
                x: 'Year',
                y: ['Country', 'Value 2 (+)'],
                color: 'Country'
            },
            title:'Title'
        } 
    }
)
},
   
    chart => chart.animate({
        config: {
            channels: {
                x: 'Joy factors',
//                label: 'Value 2 (+)'
            },
            title:'Title'
        }
    }
)
];

export default testSteps;