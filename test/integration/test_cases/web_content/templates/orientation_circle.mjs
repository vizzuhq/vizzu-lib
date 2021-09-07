import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: {
                channels:
                {
                    x: { set: ['Value 5 (+/-)'], range: {min:'-10%', max:'110%'} },
                    y: { set: ['Joy factors'], range: {min:'-10%', max:'110%'} },
                    lightness: { set: ['Year'] }
                },
                title: 'Distribution by Y',
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
                    y: { set: ['Value 5 (+/-)'] },
                    noop: { set: ['Joy factors'] }
                },
                title: 'Distribution by X'
            }
        }
    )
];

export default testSteps;