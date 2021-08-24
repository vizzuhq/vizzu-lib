import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    x: { attach: ['Neg_huge'] },
                    y: { attach: ['$exists', 'Negative nums'], range: '0,1.1,%' },
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