import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: chart.constructor.presets.polarScatter({
                angle:'Value 3 (+)',
                radius:'Value 2 (+)',
                dividedBy:'Country',
                title: 'Polar Scatter Plot'
              }),
            style: {
                plot: {
                    paddingLeft: '3.8em',
                    marker: { guides: {
                            color: null,
                            lineWidth: 0 }
                    },
                    yAxis: {
                        label: {
                           paddingRight: '0em'
                        },
                        color: '#ffffff00',
                        ticks: { color: '#ffffff00' }
                    },
                    xAxis: {
                      title: { paddingTop: '2.4em' },
                      label: { paddingTop: '0.8em' },
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;