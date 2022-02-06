import { data } from '../../../test_data/chart_types_eu.mjs';

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
                /* The noop channel splits the markers as all the other channels
                but will have no effect on the markers’ appearance. */
                noop: { set: ['Country'] }
            },
            title: 'Column Chart',
            geometry: 'rectangle'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: {
                    /* Making the chart elements fill the whole of the y-axis
                    as the default value is now 110% */
                    range: {
                        max: '100%'  
                    }
                },
                color: { set: ['Country'] },
                noop: { set: null }
            },
            title: 'Trellis Column Chart',
            split: true,
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: {
                    /* Setting back the y-axis range to the default value. */
                    range: {
                        max: 'auto'
                    }
                }
            },
            title: 'Stacked Column Chart',
            split: false
        }
    }),
    chart => chart.animate({
        config: {
            title: '100% Column Chart',
            align: 'stretch'
        }
    })
];

export default testSteps;