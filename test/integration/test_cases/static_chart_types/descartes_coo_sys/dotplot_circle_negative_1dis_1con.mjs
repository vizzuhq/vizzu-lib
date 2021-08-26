import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    x: { attach: ['Year', 'Negative nums'] },
                    y: { attach: ['$exists'], range: '0,2.0,%'}
                },
                title: 'DotPlot with (-) Nums',
                align: 'none',
                geometry: 'circle'
            }
        }
    )
];

export default testSteps;