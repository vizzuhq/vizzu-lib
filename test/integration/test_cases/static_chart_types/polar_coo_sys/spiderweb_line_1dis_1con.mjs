import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    x: { attach: ['Year'] },
                    y: { attach: ['$exists', 'Positive nums'], range: '0,1.1,%' },
                    label: { attach: ['Positive nums'] }
                },
                title: 'Spiderweb 1Line (fake. no multiaxes)',
                align: 'none',
                geometry: 'line',
                coordSystem: 'polar'
            },
            style:
            {
                plot: {
                    marker: {
                        label:                         
                        {
                            position: 'top',
                            orientation: 'tangential',
                            angle: 3.14*-0.5,
                            filter: 'lightness(0)' 
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;