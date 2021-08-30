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
                    lightness: { attach: ['Value 5 (+/-)'] },
                    size: { attach: ['Value 4 (+/-)'] },
                    label: { attach: ['Value 5 (+/-)'] }
                },
                title: 'Lightness Continuous Anim.',
                legend: 'lightness',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {

            data: {
                filter: record => record.Country != 'Germany' && record.Country != 'Italy' && record.Country != 'Hungary' && record.Country != 'Netherlands',
            },

            descriptor: {
                title: 'Lightness Continuous Anim. - Filtered'

            }
        }
    )
];

export default testSteps;