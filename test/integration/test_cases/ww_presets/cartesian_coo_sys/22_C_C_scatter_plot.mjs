import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: 'Value 6 (+/-)' },
                    y: { attach: 'Value 5 (+/-)' },
                    noop: { attach: ['Year'] }
                },
                title: 'Scatterplot with (-) Nums',
                legend: null,
                geometry: 'circle',
                coordSystem: 'cartesian'
            },
            style: {
                plot: {
                    paddingLeft: '70',
                    yAxis: {
                        label: {
                           paddingRight: 10
                        }
                    },
                    xAxis: {
                      title: { paddingTop: 30 },
                      label: { paddingTop: 10 }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;