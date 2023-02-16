import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: 'Year',
                    y: 'Value 6 (+/-)'
                },
                title: 'Line Chart with (-)',
                geometry: 'line'
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;