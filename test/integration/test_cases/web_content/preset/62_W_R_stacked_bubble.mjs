import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: chart.constructor.presets.stackedBubble({
                size:'Value 2 (+)',
                color:'Joy factors',
                stackedBy:'Country_code',
                title: 'Stacked Bubble Chart'
              }),
            style: {
                plot: {
                    paddingLeft: '0em',
                    paddingRight: '12.42em',
                    },
                legend:{ maxWidth: '20%' },
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;