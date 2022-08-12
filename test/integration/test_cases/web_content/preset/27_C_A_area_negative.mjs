import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: chart.constructor.presets.area({
                x: 'Year',
                y: 'Value 5 (+/-)',
                title: 'Area Chart'
            }),
            style: {
                plot: {
                    paddingLeft: '8em',
                    yAxis: {
                        label: {
                            paddingRight: '0.8em'
                        }
                    },
                    xAxis: {
                        title: { paddingTop: '2.4em' },
                        label: { paddingTop: '0.8em' }
                    }
                }
            }
        }
    )
];

export default testSteps;