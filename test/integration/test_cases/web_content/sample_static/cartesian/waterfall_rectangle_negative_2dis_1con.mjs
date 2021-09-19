import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: Object.assign(data, {
                filter: record => record.Country == 'Belgium'
            }),
            config:
            {
                channels:
                {
                    y: { attach: ['Year', 'Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
                    x: { attach: ['Year'] },
                    color: { attach: ['Country'] },
                    label: { attach: ['Value 5 (+/-)'] },
                    lightness: { attach: ['Year'] }
                },
                title: 'Waterfall Chart',
                legend: null
            },
            style:
            {
                plot: {
                    marker: {
                        label: {
                            position: 'top'
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;