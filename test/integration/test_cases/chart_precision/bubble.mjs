import { data } from '/test/integration/test_data/chart_precision.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    label: { attach: ['Values parent'] },
                    size: { attach: ['Values parent', 'Parents'] },
                    color: { attach: ['Childs'] }
                },
                title: 'Bubble',
                geometry: 'circle'
            }
        }
    )
];

export default testSteps;