import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
    plot: { marker: { label: { fontSize: 14 } } },
    data: { barMaxPadding: 0.3, barPaddingDecrease: 5.5 }
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
                    y: { range: '1,-3,%' },
                    color: { attach: ['Threat'] },
                    label: { attach: ['Positive nums'] }
                },
                title: 'Donut Chart',
                legend: 'color',
                orientation: 'vertical',
                coordSystem: 'polar'
            },
            style: styles
        }

    )
];

export default testSteps;