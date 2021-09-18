import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                color: { set: ['Joy factors'] },
                label: { set: ['Country_code'] },
                size: { set: ['Country_code', 'Value 2 (+)'] }
            },
            title: 'Elements',
            geometry: 'circle'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Joy factors'] },
                y: {
                    set: ['Country_code', 'Value 2 (+)'],
                    range: {
                        min: '0%',
                        max: '110%'
                    }
                },
                label: { set: null },
                size: { set: null }
            },
            title: 'Total',
            geometry: 'rectangle',
            orientation: 'vertical'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: { detach: ['Country_code'] },
                label: { set: ['Value 2 (+)'] }
            }
        }
    })
];

export default testSteps;