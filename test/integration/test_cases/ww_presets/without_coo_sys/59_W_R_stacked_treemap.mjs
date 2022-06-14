import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    color: { attach: 'Joy factors' },
                    size: { attach: ['Value 2 (+)', 'Country_code'] },
                    label: { attach: 'Country_code' },
                    lightness: { attach: 'Value 2 (+)' }
                },
                title: 'Stacked Treemap'
            },
            style: {
                plot: {
                    paddingLeft: 0,
                    marker: { 
                        label: { fontSize: '0.7em'} 
                        }
                    },
                legend:{ maxWidth: '20%' },
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;