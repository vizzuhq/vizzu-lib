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
            config:
            {
                channels:
                {
                    y: { attach: ['Value 1 (+)'], range: '0,1.1,%' },
                    x: { attach: ['Country', 'Value 2 (+)'] },
                    color: { attach: ['Country'] },
                    label: { attach: ['Value 2 (+)', 'Country'] }
                },
                title: 'Mekko chart'
            }
        }
    )
];

export default testSteps;