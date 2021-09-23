import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                ['AT', 'BE', 'DE', 'DK', 'ES', 'FI', 'FR', 'IT', 'NL', 'SE']
                .includes(record.Country_code)
        }),
        config: {
            channels: {
                x: { set: ['Year', 'Joy factors'] },
                y: { set: ['Value 3 (+)'] },
                color: { set: ['Country_code'] }
            },
            title: 'See all',
            geometry: 'line'
        }
    }),
    chart => chart.animate({
        data: { 
            filter: record => data.filter(record) 
                && record.Year < 8 && record.Year > 2 
        },
        config:
        {
            title: 'Zoom in',
            align: 'min'
        }
    })
];

export default testSteps;