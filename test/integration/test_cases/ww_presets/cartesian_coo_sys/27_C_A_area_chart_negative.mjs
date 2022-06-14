import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: ['Year'] },
                    y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } }
                },
                title: 'Area Chart with (-) Nums',
                align: 'none',
                geometry: 'area'
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