import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
    data: { barMaxPadding: 0.25 },
    plot: { axis: { label: { paddingRight: 20, fontSize: 12 } } }
};

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Threat'] },
                    x: { attach: ['Positive nums'], range: '0,1.33,%' },
                    color: { attach: ['Threat'] }
                },
                title: 'Radial Chart',
                coordSystem: 'polar',
                legend: null
            },
            style: styles
        }
    )
];

export default testSteps;