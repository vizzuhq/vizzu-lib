import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                color: { set: ['Joy factors'] },
                label: { set: ['Country_code'] },
                size: { set: ['Country_code', 'Value 2 (+)'] } // Put dimension data series on 'size' channel to stack by other dimension which is on 'color' channel. 
            },
            title: 'Elements',
            geometry: 'circle'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Country_code', 'Value 2 (+)'] },
                y: { set: ['Joy factors'] },
                label: { set: null },
                size: { set: null } // Take dimension data series off from 'size' channel to terminate stacking. 
            },
            title: 'Total',
            geometry: 'rectangle',
            orientation: 'vertical'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { detach: ['Country_code'] },
                label: { set: ['Value 2 (+)'] }
            }
        }
    })
];

export default testSteps;