import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: ['Value 6 (+/-)'] },
                    y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
                    noop: { attach: ['Year'] },
                    label: { attach: ['Year'] }
                },
                title: 'Scatterplot with (-) Nums',
                legend: null,
                geometry: 'circle',
                coordSystem: 'cartesian'
            }
        }
    )
];

export default testSteps;