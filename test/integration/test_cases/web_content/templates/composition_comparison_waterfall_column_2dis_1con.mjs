import { data } from '/test/integration/test_data/chart_types_eu.js';

data.filter = record => 
record.Country == 'Belgium';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Year', 'Value 5 (+/-)'], range: '0,1.1,%' },
                    x: { attach: ['Year'] },
                    color: { attach: ['Value 5 (+/-)'], range: '-45, 45, 1'  },
                    noop: { attach: ['Country'] },
                    label: { attach: ['Value 5 (+/-)'] }
                },
                title: 'Waterfall Chart',
                legend: 'color'
            },
            style:
            {
                plot: {
                    marker: {
                        label: {
                            position: 'top'
                        }
                    }
                },
                data: {colorGradient:'#f23434 0, #f23434 0.5, #6467bf 0.5, #6467bf 1'}
            }

        }
    ),
    chart => chart.animate(
        {
            config:
            {
                channels:
                {
                    y: { set: ['Value 5 (+/-)'], range: '-0.1,1.1,%' }
                },
                title: 'Column Chart'
            },
            style:
            {
                plot: {
                    marker: {
                        label: {
                            position: 'top'
                        }
                    }
                },
                data: {colorGradient:'#f23434 0, #f23434 0.5, #6467bf 0.5, #6467bf 1'}
            }
        }
    )
];

export default testSteps;