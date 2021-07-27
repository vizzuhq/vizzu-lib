import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
    data: { columnMaxPadding: 0.15 },
    plot: {
        paddingLeft: 100,
        marker: {
            borderWidth: 0,
        }
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
                    y: { attach: ['Country', 'Year', 'Negative nums'], range: '0,1.1,%' },
                    x: { attach: ['Country_code'] },
                    color: { attach: ['Country'] },
                    lightness: { attach: ['Negative nums'] },
                },
                title: 'Stacked Column Chart with (-) Numbers',
                legend: 'lightness',
            },
            style: styles
        }
    )
];

export default testSteps;

