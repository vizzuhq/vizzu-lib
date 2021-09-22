import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                record.Country == 'Belgium'
        }),
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Year', 'Value 5 (+/-)'] }, // Repeated data series (Year) on both axles builds up waterfall positions of the markers. 
                color: { set: ['Country'] },
                label: { set: ['Value 5 (+/-)'] }
            },
            title: 'Waterfall Chart',
            legend: null
        },
        style: {
            plot: {
                marker: {
                    label: {
                        position: 'top'
                    }
                }
            }
        }
    })
];

export default testSteps;