import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: chart.constructor.presets.bubbleplot({
            x:'Value 4 (+/-)',
            y:'Value 5 (+/-)',
            color:'Joy factors',
            noop:'Country',
            size:'Value 3 (+)',
            title: 'Bubble Plot'
          }),
        style:
        {
            plot: {
                paddingLeft: '1.2em',
                yAxis: {
                label: { paddingRight: '0.8em' }
                },
            xAxis: {
                title: { paddingTop: '2.4em' },
                label: { paddingTop: '0.8em' }
                }
            }
        }
    }),
    chart => chart.feature('tooltip',true)
];

export default testSteps;