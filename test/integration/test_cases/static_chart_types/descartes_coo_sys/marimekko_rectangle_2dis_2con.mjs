import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
    plot: {
        marker: {
            borderWidth: 0,
            label: {
                fontSize: 12,
                format: 'categoriesFirst'
            }
        }
    }
};

data.filter = record => record.Country == 'Bulgaria' || record.Country == 'Germany' || record.Country == 'Malta' || record.Country == 'Lithuania'

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Threat', 'Positive nums'], range: '0,1.1,%' },
                    x: { attach: ['Country', 'Positive nums'] },
                    color: { attach: ['Threat'] },
                    label: { attach: ['Positive nums', 'Country'] }
                },
                title: 'Stacked Mekko chart',
                legend: null
            },
            style: styles
        }
    )
];

export default testSteps;