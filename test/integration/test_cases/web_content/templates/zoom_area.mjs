import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                ['AT', 'BE', 'DE', 'DK', 'ES', 'FI', 'FR', 'IT', 'NL', 'SE']
                .includes(record.Country_code)
        }),
        config: {
            channels: {
                x: ['Year', 'Joy factors'],
                y: ['Value 3 (+)', 'Country_code'],
                color: 'Country_code'
            },
            title: 'Stacked Area',
            geometry: 'area'
        }
    }),
    chart => chart.animate(
        {
            data: { 
                filter: record => data.filter(record) 
                    && record.Year < 12 && record.Year > 6 
            }
        })
];

export default testSteps;