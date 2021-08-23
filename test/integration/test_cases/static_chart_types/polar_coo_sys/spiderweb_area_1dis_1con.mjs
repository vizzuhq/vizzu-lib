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
                    y: { attach: ['$exists', 'Positive nums'], range: '0,1.3,%' },
                    label: { attach: ['Positive nums'] },
                },
                title: 'Spiderweb Area (not a real Spider)',
                legend: null,
                geometry: 'area',
                coordSystem: 'polar',
            },
            style: 
            {
                plot: {
                    marker: {
                        label:                         
                        {
                            orientation: 'tangential',
                            angle: 3.14*-0.5
                        }
                    },
                    yAxis:
                    { 
                        title:
                        {
                            paddingBottom: '25' 
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;