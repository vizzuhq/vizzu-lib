import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: 'Year' },
                    y: { attach: 'Value 1 (+)' },
//                    label: { attach: 'Value 1 (+)' }
                },
                title: 'Lollipop Chart',
                align: 'none',
                geometry: 'circle'
            },
            style:
            {
                plot: {
                    paddingLeft: '8em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                    },
                xAxis: {
                    title: { paddingTop: '2.4em' },
                    label: { paddingTop: '0.8em' }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;