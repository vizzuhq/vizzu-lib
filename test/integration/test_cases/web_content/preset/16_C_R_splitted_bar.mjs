import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: chart.constructor.presets.splittedBar({
                x:'Value 2 (+)',
                y:'Year',
                splittedBy:'Joy factors',
                title: 'Splitted Bar Chart'
              }),
            style: {
                plot: {
                    paddingLeft: '1.2em',
                    yAxis: {
                        label: {
                           paddingRight: '0.8em'
                        }
                    },
                    xAxis: {
                        label: { paddingTop: '0.8em' },
                        title: { paddingTop: '2.4em' }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;