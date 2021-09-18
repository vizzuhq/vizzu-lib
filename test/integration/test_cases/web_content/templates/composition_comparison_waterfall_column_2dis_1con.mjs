import { data } from '/test/integration/test_data/chart_types_eu.js';

data.filter = record =>
    record.Country == 'Belgium';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                y: {
                    attach: ['Year', 'Value 5 (+/-)'],
                    range: {
                        min: '0%',
                        max: '110%'
                    }
                },
                x: { attach: ['Year'] },
                color: {
                    attach: ['Value 5 (+/-)'],
                    range: {
                        min: '-45',
                        max: '45'
                    }
                },
                noop: { attach: ['Country'] },
                label: { attach: ['Value 5 (+/-)'] }
            },
            title: 'Waterfall Chart',
            legend: 'color'
        },
        style: {
            plot: {
                marker: {
                    colorGradient: '#3d51b8 0, #6389ec 0.15, #9fbffa 0.35, #d5d7d9 0.5, #f4b096 0.65, #e36c56 0.85, #ac1727 1' /*'#f23434 0, #f23434 0.5, #6467bf 0.5, #6467bf 1'*/,
                    label: {
                        position: 'top'
                    }
                }
            }
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: {
                    set: ['Value 5 (+/-)'],
                    range: {
                        min: '-10%',
                        max: '110%'
                    }
                }
            },
            title: 'Column Chart'
        }
    })
];

export default testSteps;