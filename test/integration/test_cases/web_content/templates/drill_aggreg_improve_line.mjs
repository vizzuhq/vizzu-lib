import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                ['AT', 'DE', 'FI', 'ES']
                .includes(record.Country_code)
        }),
        config: {
            channels: {
                x: { set: ['Year'] },
                y: {
                    set: ['Value 3 (+)'],
                    /* Making the chart elements fill the whole of
                    the y-axis as the default value is now 110% */
                    range: {
                        max: '1100000000' 
                    }
                },
                
            },
            title: 'Line Chart',
            geometry: 'line'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: { attach: ['Country'] },
                color: { set: ['Country'] }
            },
            title: 'Drill down',
            geometry: 'area'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: { detach: ['Country']}
            },
            title: 'Line Chart',
            geometry: 'line'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: {
                    /* Setting back the y-axis range
                    to the default value. */
                    range: {
                        max: 'auto' 
                    }
                }
            }
        }
    })
];

export default testSteps;