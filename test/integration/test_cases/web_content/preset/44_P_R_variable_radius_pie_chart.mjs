import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: chart.constructor.presets.variableRadiusPie({
                angle:'Value 2 (+)',
                radius: 'Value 1 (+)',
                by:'Joy factors',
                title: 'Variable Radius Pie Chart'
              }),
            style: {
                plot: {
                    paddingLeft:'0em',
                    paddingRight:'12.42em',
                    xAxis: {
                        ticks: { color: '#ffffff00' }
                    },
                    yAxis:
                    {
                        color: '#ffffff00',
                        label: { paddingRight: '0' }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;