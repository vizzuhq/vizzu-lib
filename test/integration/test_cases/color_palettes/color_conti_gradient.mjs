import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Value 5 (+/-)'], range: '0,1.5,%' },
                    x: { attach: ['Value 6 (+/-)'], range: '0,1.5,%' },
                    color: { attach: ['Value 5 (+/-)'] },
                    size: { attach: ['Value 4 (+/-)'] },
                    label: { attach: ['Value 5 (+/-)'] },
                    noop: { attach: ['Country'] } 
                },
                title: 'Default Color Gradient',
                legend: 'color',
                geometry: 'circle'
            }
        }
    )
];

export default testSteps;