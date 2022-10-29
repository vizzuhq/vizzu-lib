import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: chart.constructor.presets.percentageArea({
                x: 'Year',
                y: 'Value 2 (+)',
                stackedBy: 'Country',
                title: 'Percentage Area Chart'
            }),
            style: {
                plot: {
                    paddingLeft: '1.2em',
                    yAxis: {
                        label: { paddingRight: '0.8em' }
                    },
                    xAxis: {
                        label: { paddingTop: '0.8em' }
                    }
                }
            }
        }
    )
];

export default testSteps;