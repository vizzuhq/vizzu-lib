import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate( {
            data: Object.assign(data, {
                filter: record =>
                [ 'AT', 'BE', 'CY', 'DE', 'DK', 'EE', 'EL', 'ES', 'FI', 'FR', 'IT', 'NL', 'SE' ]
                .includes(record.Country_code)
            }),
            config: {
                channels: {
                    x: { set: ['Year'] },
                    y: { set: ['Value 2 (+)', 'Country'] },
                    noop: { set: ['Country'] }
                },
                title: 'Column Chart',
                geometry: 'rectangle'
            }
        }),
    chart => chart.animate( {            
            config: {
                channels: {
                    color: { set: ['Country'] },
                    noop: { set: null }
                },
                title: 'Drill down & check the elements separatelly',
                split:true,
            }
        }),
    chart => chart.animate( {
            config: {
                title: '...or together',
                align: 'min',
                split:false
            }
        }),
    chart => chart.animate({
            config: {                
            title: '...or the relative differences between these',
            align: 'stretch',
            split:false
            }
        })
];

export default testSteps;