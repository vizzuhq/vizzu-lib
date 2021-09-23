import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Year', 'Value 5 (+/-)'] }, // Repeated data series (Year) on both axles builds up waterfall positions of the markers. 
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