import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { set: ['Country', 'Value 2 (+)'], range: '0,1.1,%' },
                    x: { set: ['Year'] },
                    color: { set: ['Country'] },
                },
                title: 'Time distribution',
                geometry: 'area'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: 
            {
                channels:
                {
                    y: { range: '0,1,%' }
                },
                split: true
            }
        },
        {delay: 0}
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                channels:
                {
                    y: { detach: ['Value 2 (+)'] },
                    x: { set: ['Value 2 (+)','Year'] }
                },
                title: 'Amount',
                geometry: 'rectangle',
                split: false
            },
            style: {
                data: {barMaxPadding: 0.13}
            }
        },
        {
//            style: { delay: 0, duration: 2 },
//            title: { delay: 0, duration: 2 },
//            legend: { delay: 0, duration: 2 },
//            show: { delay: 0, duration: 2 },
//            hide: { delay: 0, duration: 2 },
//            color: { delay: 0, duration: 2 },
//            coordSystem: { delay: 2, duration: 2 },
            geometry: { delay: 0, duration: 1 },
            y: { delay: 0, duration: 1 },
            x: { delay: 0, duration: 1 }
        }
    )
];

export default testSteps;