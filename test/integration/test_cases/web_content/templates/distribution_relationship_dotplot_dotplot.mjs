import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Joy factors'] },
                y: { set: ['Value 5 (+/-)'] },
                color: { set: ['Joy factors'] },
                /* The noop channel splits the markers as all the other channels
                but will have no effect on the markers’ appearance. */
                noop: { set: ['Country_code'] } 
            },
            title: 'Distribution Plot',
            geometry: 'circle'
        },
        style: {
            plot: {
                marker: {
                    label: { fontSize: 9 }
                }
            }
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Joy factors', 'Value 6 (+/-)'] },
                label: { set: ['Country_code'] }
            },
            title: 'Scatter Plot'
        }
    })
];

export default testSteps;