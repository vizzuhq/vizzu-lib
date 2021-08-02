
import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
    data: { barMaxPadding: 0.25 },
    plot: {
        axis: { label: { paddingRight: 20, fontSize: 12 } },
        marker: { borderWidth: 1, borderOpacity: 1 }
    }
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
                    x: { attach: ['Country', 'Positive nums'], range: '0,1.33,%' },
                    color: { attach: ['Threat'] }
                },
                title: 'Radial Chart (2disc)',
                coordSystem: 'polar',
                orientation: 'vertical',
                align: 'min'
            },
            style: styles
        }
    )
];

export default testSteps;