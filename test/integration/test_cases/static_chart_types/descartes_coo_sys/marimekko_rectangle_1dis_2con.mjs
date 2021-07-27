import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
    plot: {
        marker: {
            label: {
                fontSize: 13,
                position: 'above',
                filter: 'lightness(0)',
                format: 'categoriesFirst'
            }

        }
    }
};

data.filter = record => record.Country == 'Bulgaria' || record.Country == 'Germany' || record.Country == 'Malta' || record.Country == 'Lithuania' // Csak ezek látszódjanak.;

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Pos_small'], range: '0,1.1,%' },
                    x: { attach: ['Country', 'Positive nums'] },
                    color: { attach: ['Country'] },
                    label: { attach: ['Positive nums', 'Country'] }
                },
                title: 'Mekko chart',
                legend: null
            },
            style: styles
        }
    )
];

export default testSteps;