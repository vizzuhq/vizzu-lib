import { data } from '../../../test_data/chart_types_eu.mjs';

data.filter = record => 
record.Country == 'Belgium';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: 'Year' },
                    y: { attach: ['Year', 'Value 5 (+/-)']},
                    color: { attach: 'Value 5 (+/-)', range: {min:Number.MIN_SAFE_INTEGER, max:Number.MAX_SAFE_INTEGER} },
                    label: { attach: 'Value 5 (+/-)' }
                },
                title: 'Waterfall Chart',
                legend: null
            },
            style:
            {
                plot: {
                    paddingLeft: '8em',
                    marker: {
                        colorGradient: 
                        [
                            '#ff001b 0',
                            '#ff001b 0.5',
                            '#7e79e8 0.5',
                            '#7e79e8 1'
                        ].join(),
                        label: {
                            position: 'top'
                        }
                    },
                    yAxis: {
                        label: {
                           paddingRight: '0.8em'
                        }
                    },
                    xAxis: {
                        label: { paddingTop: '0.8em' }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;