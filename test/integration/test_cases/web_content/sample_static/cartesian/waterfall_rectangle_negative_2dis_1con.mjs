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
                    y: { set: ['Year', 'Value 5 (+/-)'], range: '0,1.1,%' },
                    x: { set: ['Year'] },
                    color: { set: ['Country'] },
                    label: { set: ['Value 5 (+/-)'] },
                    lightness: { set: ['Year'] }
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