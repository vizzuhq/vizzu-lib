import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
    plot: { marker: { label: { fontSize: 16 } } }
};

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    x: { attach: ['Threat', 'Positive nums'] },
                    y: { title: 'PRÓBA', attach: ['index'] },
                    color: { attach: ['Threat'] },
                    label: { attach: ['Positive nums'] }
                },
                title: 'Pie Chart',
                geometry: 'rectangle',
                coordSystem: 'polar',
                legend: null
            },
            style: styles
        }
    )
];

export default testSteps;