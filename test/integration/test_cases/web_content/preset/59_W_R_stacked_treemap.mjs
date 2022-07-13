import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: chart.constructor.presets.treemap({
                size:'Value 2 (+)',
                color:'Joy factors',
                lightness:'Value 2 (+)',
                title: 'Stacked Treemap'
              }),
            style: {
                plot: {
                    paddingLeft: '0em',
                    marker: { 
                        label: { fontSize: '0.7em'} 
                        }
                    },
                legend:{ maxWidth: '20%' },
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;