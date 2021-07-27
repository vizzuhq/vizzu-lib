import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
    data: { columnMaxPadding: 0.347 },
    plot: { marker: { label: { fontSize: 14 } } }
};

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Positive nums'] },
                    x: { attach: ['Threat'] },
                    color: { attach: ['Threat'] },
                    label: { attach: ['Positive nums'] }
                },
                title: 'Coxcomb Chart',
                orientation: 'horizontal',
                coordSystem: 'polar',
                legend: null
            },
            style: styles
        }
    )
];

export default testSteps;