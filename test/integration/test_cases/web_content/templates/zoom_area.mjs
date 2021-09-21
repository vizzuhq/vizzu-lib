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
                y: {
                    set: ['Value 3 (+)', 'Country_code'],
                    range: {
                        min: '0%',
                        max: '110%'
                    }
                },
                color: { set: ['Country_code'] }
            },
            title: 'Zoom in',
            geometry: 'area'
        }
    }),
    chart => chart.animate(
        {
            data: { 
                filter: record => data.filter(record) 
                    && record.Year < 12 && record.Year > 6 
            },
            config: {
                title: 'Zoomed in',
                align: 'min'
            }
        })
];

export default testSteps;