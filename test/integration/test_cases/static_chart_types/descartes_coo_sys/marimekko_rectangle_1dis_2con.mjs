import { data } from '/test/integration/test_data/chart_types_eu.js';

data.filter = record => 
record.Country == 'Bulgaria' || 
record.Country == 'Germany' || 
record.Country == 'Malta' || 
record.Country == 'Lithuania' // Csak ezek látszódjanak.;

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
            }
        }
    )
];

export default testSteps;