import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: chart.constructor.presets.pie({
                angle: 'Value 2 (+)',
                by: 'Joy factors',
                title: 'Pie Chart'
            }),
            style:
            {
                plot: {
                    paddingLeft: '0em',
                    paddingRight: '12.42em',
                    xAxis: {
                        title: { color: '#ffffff00' },
                        label: { color: '#ffffff00' },
                        interlacing: { color: '#ffffff00' }
                    },
                    yAxis: {
                        color: '#ffffff00'
                    }
                }
            }
        }
    )
];

export default testSteps;