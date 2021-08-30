import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Value 5 (+/-)'], range: '0,1.1,%' },
                    x: { attach: ['Value 6 (+/-)'], range: '0,1.1,%' },
                    color: { attach: ['Country'] },
                    size: { attach: ['Value 4 (+/-)'] },
                    label: { attach: ['Value 4 (+/-)'] }
                },
                title: 'Size Continuous Anim.',
                legend: 'size',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {

            data: {
                filter: record => 
                record.Country != 'Germany' && 
                record.Country != 'Italy' && 
                record.Country != 'Hungary' && 
                record.Country != 'Netherlands' && 
                record.Country != 'Finland' && 
                record.Country != 'Lithuania' && 
                record.Country != 'Latvia' && 
                record.Country != 'Croatia' && 
                record.Country != 'Ireland' && 
                record.Country != 'Denmark' && 
                record.Country != 'Hungary' && 
                record.Country != 'United Kingdom',
            },

            descriptor: {
                title: 'Size Continuous Anim. - Filtered'
            }
        }
    )
];

export default testSteps;