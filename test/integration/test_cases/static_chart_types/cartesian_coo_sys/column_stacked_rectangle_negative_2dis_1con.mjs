import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Country', 'Year', 'Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
                    x: { attach: ['Country_code'] },
                    color: { attach: ['Country_code'] },
                    lightness: { attach: ['Value 5 (+/-)'] },
                },
                title: 'Stacked Column Chart with (-) Numbers',
                legend: 'lightness'
            },
            style: {
                plot: 
                {
                    marker: 
                    {
                        borderWidth: 0,
                        borderOpacity: 1
                    }
                }
            }
        }
    )
];

export default testSteps;

