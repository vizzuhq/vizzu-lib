import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Year'] },
                y: {
                    set: ['Value 3 (+)'],
                    range: {
                        max: '6000000000' // Keep the yAxis-range during the next animation phase.
                    }
                },
                size: { set: ['Country'] } // Set the other categories of the chart.
            },
            title: 'Line Chart',
            geometry: 'line'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                color: { set: ['Country'] },
                size: { set: null } // You don't need the categories on this channel, because they are on the 'color' channel already.
            },
            title: 'Drill Down'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: {
                    range: {
                        max: '110%' // Set back the default yAxis-range.
                    }
                },
            },
            title: 'Drilled Line Chart'
        }
    })
];

export default testSteps;