import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: {
                    attach: ['Value 6 (+/-)'],
                    range: {
                        min: '-10%',
                        max: '110%'
                    }
                },
                y: {
                    attach: ['$exists', 'Value 5 (+/-)'],
                    range: {
                        min: '-10%',
                        max: '110%'
                    }
                },
                noop: { attach: ['Year'] },
                label: { attach: ['Year'] }
            },
            title: 'Scatter Plot',
            legend: null,
            geometry: 'circle',
            coordSystem: 'cartesian'
        }
    })
];

export default testSteps;