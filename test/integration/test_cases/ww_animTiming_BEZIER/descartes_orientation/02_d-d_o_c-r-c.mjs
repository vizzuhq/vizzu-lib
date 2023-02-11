import { data } from '../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                record.Country == 'Austria' ||
                record.Country == 'Belgium' ||
                record.Country == 'Bulgaria' ||
                record.Country == 'Cyprus' ||
                record.Country == 'Czechia' ||
                record.Country == 'Germany' ||
                record.Country == 'Denmark' ||
                record.Country == 'Estonia' ||
                record.Country == 'Greece' ||
                record.Country == 'Spain' ||
                record.Country == 'Finland' ||
                record.Country == 'France' ||
                record.Country == 'Croatia' ||
               record.Country == 'Hungary'
        }),
        config: {
            channels: {
                x: ['Value 5 (+/-)','Year'],
                y: 'Value 2 (+)',
                color: 'Country',
            },
            title: 'Stacked Area Chart',
            geometry: 'circle',
            legend: null
        } 
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: ['Value 2 (+)', 'Year'],
                y: 'Country',
                color: 'Country',
            },
            title: 'Bar Chart',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false,
        }
    },
    {
         geometry: {
            easing: 'cubic-bezier(1,0,.7,.1)',
        }
          /*,
        x: {
            easing: 'step-start'
        },
        y: {
            easing: 'step-start'
        },
        style: {
            easing: 'step-start'
        }
            */
     }
 
    ),
    chart => chart.animate({
    config: {
            channels: {
                x: 'Value 4 (+/-)',
                noop: 'Year',
                y: 'Value 2 (+)',
                color: 'Country',
            },
            title: 'Scatter Plot Chart',
            geometry: 'circle',
            legend: null
        } 
    },
    {
         geometry: {
            easing: 'cubic-bezier(.15,0,0,1)',
            duration: 2
        }
        /*,
        x: {
            easing: 'step-end'
        },
        y: {
            easing: 'step-end'
        },
        style: {
            easing: 'step-end'
     }
     */
    }
    ),
];

export default testSteps;