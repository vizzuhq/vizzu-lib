import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: chart.constructor.presets.splittedColumn({
                x:'Year',
                y:'Value 2 (+)',
                splittedBy: 'Joy factors',
                title: 'Splitted Column Chart'
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
                        label: { paddingTop: '0.8em' }
                    }
                }
            }
        }
    ),

    chart => chart.feature('tooltip',true)
];

export default testSteps;