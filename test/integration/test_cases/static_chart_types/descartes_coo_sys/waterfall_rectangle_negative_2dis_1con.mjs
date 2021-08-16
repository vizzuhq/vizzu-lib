import { data } from '/test/integration/test_data/chart_types_eu.js';

data.filter = record => record.Country == 'Belgium';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Year', 'Negative nums'], range: '0,1.1,%' },
                    x: { attach: ['Year'] },
                    color: { attach: ['Country'] },
                    label: { attach: ['Negative nums'] },
                    lightness: { attach: ['Year'] }
                },
                title: 'Waterfall Chart with (-) Nums (Belgium)',
                legend: null
            },
            style:
            {
                plot: {
                    marker: {
                        label: {
                            position: 'top',
                            filter: 'lightness(0)'
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;