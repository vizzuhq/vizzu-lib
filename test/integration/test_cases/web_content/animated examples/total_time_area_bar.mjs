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
                x: 'Year',
                y: ['Country', 'Value 2 (+)'],
                color: 'Country',
            },
            title: 'Stacked Area Chart',
            geometry: 'area'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: {
                    /* Making the chart elements fill the whole of
                       the y-axis as the default value is now 110% */
                    range: {
                        max: '100%' 
                    }
                }
            },
            title: 'Trellis Area Chart',
            split: true
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: ['Value 2 (+)', 'Year'],
                y: 'Country'
            },
            title: 'Bar Chart',
            geometry: 'rectangle',
            split: false,
        }
    },
        {
            /* Setting a custom rhythm for the animation
            to assist the viewer in following it. */
            geometry: { delay: 0, duration: 1 }, 
            y: { delay: 0, duration: 1 },
            x: { delay: 0, duration: 1 }
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: 'Value 2 (+)',
                label: 'Value 2 (+)'
            }
        }
    })
];

export default testSteps;