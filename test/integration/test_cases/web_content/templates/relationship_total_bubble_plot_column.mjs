import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: {
                channels:
                {
                    x: { set: ['Joy factors', 'Value 6 (+/-)'], range: {min:'-10%', max:'110%'} },
                    y: { set: ['Value 5 (+/-)'], range: {min:'-10%', max:'110%'} },
                    color: { set: ['Joy factors'] },
                    size: { set: ['Value 2 (+)'] },
                    label: { set: ['Country_code'] }
                },
                title: 'Realationship',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            config: {
                channels:
                {
                    y: { set: ['Joy factors'] },
                    x: { set: ['Value 2 (+)','Country_code'], range: {min:'0%', max:'110%'} },
                    label: { set: null }
                },
                title: 'Total',
                geometry: 'rectangle',
                orientation: 'vertical'
            }
        },
        {
            geometry: 
            { 
                delay: 0.7, 
                duration: 1 
            }
        }
    ),
    chart => chart.animate(
        {
            config: {
                channels:
                {
                    x: { set: ['Value 2 (+)'], range: {min:'0%', max:'110%'} },
                    label: { set: ['Value 2 (+)'] }
                },
                title: 'Total'
            }
        }
    )
];

export default testSteps;