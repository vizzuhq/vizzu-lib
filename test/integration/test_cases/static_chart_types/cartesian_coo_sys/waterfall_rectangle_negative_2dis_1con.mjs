import { data } from '../../../test_data/chart_types_eu.mjs';

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
                    y: { attach: ['Year', 'Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
                    x: { attach: ['Year'] },
                    color: { attach: ['Country'] },
                    label: { attach: ['Value 5 (+/-)'] },
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
                            position: 'top'
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;