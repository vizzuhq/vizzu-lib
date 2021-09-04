import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: ['Year'] },
                    y: { attach: ['$exists', 'Value 2 (+)'], range: { min: '0%', max: '1.3%' } },
                    label: { attach: ['Value 2 (+)'] },
                },
                title: 'Spiderweb Area (not a real Spider)',
                legend: null,
                geometry: 'area',
                coordSystem: 'polar',
            },
            style: 
            {
                plot: {
                    marker: {
                        label:                         
                        {
                            orientation: 'tangential',
                            angle: 3.14*-0.5
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;