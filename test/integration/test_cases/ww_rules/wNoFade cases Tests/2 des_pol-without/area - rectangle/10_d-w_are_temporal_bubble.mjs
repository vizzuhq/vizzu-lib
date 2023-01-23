import { data } from '../../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                record.Country == 'Austria' ||
                record.Country == 'Belgium' ||
                record.Country == 'Bulgaria' ||
                record.Country == 'Cyprus' ||
                record.Country == 'Czechia' ||
                record.Country == 'Denmark' ||
                record.Country == 'Estonia' ||
                record.Country == 'Greece' ||
                record.Country == 'Germany' ||
                record.Country == 'Spain' ||
                record.Country == 'Finland' ||
                record.Country == 'France' ||
                record.Country == 'Croatia' ||
               record.Country == 'Hungary'
        }),

        config: {
            channels: {
                x: 'Year',
                y: ['Joy factors', 'Value 2 (+)'],
                color: 'Joy factors',
                noop: 'Year',
                size: 'Value 2 (+)'
            },
            title: 'Stacked Area Chart',
            geometry: 'area',
            align: 'center',
            split: true
        }
    }
    
    ),

chart => chart.animate({
    config: {
        channels: {
            x: null,
            y: null,
            noop: 'Year',
            size: ['Year', 'Value 2 (+)'],
            color: 'Joy factors'
        },
        title: 'Bubble',
        geometry: 'rectangle'
    }
}
),

chart => chart.animate({
    config: {
        channels: {
            noop: null,
            size: ['Value 2 (+)'],
            color: 'Joy factors'
        },
        title: 'Bubble'
    }
}
)

];

export default testSteps;