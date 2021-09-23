import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
            ['AT', 'BE', 'CY', 'DE', 'DK', 'EE', 'EL', 'ES', 'FI', 'FR', 'IT', 'NL', 'SE']
            .includes(record.Country_code)
    }),
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Value 2 (+)', 'Country'] },
                color: { set: ['Country'] }
            },
            title: 'Stacked Column Chart',
            geometry: 'rectangle'
        }
    }),
    chart => chart.animate({
        config: {
            title: '100% Stacked Column Chart',
            align: 'stretch'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: {
                    range: {
                        max: '100%' // For use the whole range of the yAxis. (Default = 110%)  
                    }
                },
            },
            title: 'Trellis Column Chart',
            align: 'min',
            split: true
        }
    })
];

export default testSteps;