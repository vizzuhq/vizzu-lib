import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles =
{
    plot: {
        marker: {
            label: {
                position: 'above',
                filter: 'lightness(0)'
            }
        }
    }
};

data.filter = record => record.Country == 'Estonia';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: [/*'Country',*/ 'Year', 'Negative nums'], range: '0,1.1,%' },
                    x: { attach: ['Year'] },
                    color: { attach: ['Country'] },
                    label: { attach: ['Negative nums'] },
                    lightness: { attach: ['Year'] }
                },
                title: 'Waterfall Chart with (-) Nums (Estonia)',
                legend: null
            },
            style: styles
        }
    )
];

export default testSteps;