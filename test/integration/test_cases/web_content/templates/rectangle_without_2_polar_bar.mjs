import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    color: { set: ['Joy factors'] },
                    size: { set: ['Value 2 (+)'] },
                    label: { set: ['Joy factors'] }
                },
                title: '1 - Treemap'
            },
            style: {
                plot: {
                    marker: {
                        label: { fontSize: 14 }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    y: { set: ['Joy factors'] },
                    x: { set: ['Value 2 (+)'], range: '0,1.33,%'  },
                    size: { set: null },
                    label: { set: null }
                },
                title: '2',
                coordSystem: 'polar'
            }
        }
    ),
        chart => chart.animate(
            {
                descriptor: {
                    channels:
                    {
                        y: { set: null },
                        x: { set: null },
                        size: { set: ['Value 2 (+)'] },
                        label: { set: ['Joy factors'] }
                    },
                    title: '3',
                    coordSystem: 'cartesian'
                }
            }
    )
];

export default testSteps;