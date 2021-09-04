import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: {
                channels:
                {
                    x: { set: ['Value 5 (+/-)'], range: '-0.1,1.1,%' },
                    y: { set: ['Value 6 (+/-)'], range: '-0.1,1.1,%' },
                    noop: { set: ['Joy factors'] },
                    lightness: { set: ['Year'] }
                },
                title: 'Relationship by X, Y',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            config: {
                channels:
                {
                    x: { set: ['Year'] },
                    y: { set: ['Value 6 (+/-)'] }
                },
                title: 'Distribution by X',
                legend: 'lightness'
            }
        }
    ),
    chart => chart.animate(
        {
            config: {
                channels:
                {
                    x: { set: ['Value 5 (+/-)'] },
                    y: { set: ['Value 6 (+/-)'] }
                },
                title: 'Relationship by X, Y'
            }
        }
    ),   
    chart => chart.animate(
        {
            config: {
                channels:
                {
                    x: { set: ['Value 5 (+/-)'] },
                    y: { set: ['Joy factors'] },
                    noop: { set: null }
                },
                title: 'Distribution by Y'
            }
        }
    )
];

export default testSteps;