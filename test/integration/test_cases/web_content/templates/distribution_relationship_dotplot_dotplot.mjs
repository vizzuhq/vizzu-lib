import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Joy factors'] },
                y: { set: ['Value 5 (+/-)'] },
                color: { set: ['Joy factors'] },
                noop: { set: ['Country_code'] } //The noop channel splits the markers as all the other channels but will have no effect on the markers’ appearance.
            },
            title: 'Dot Plot',
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