import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: chart.constructor.presets.stackedTreemap({
                size: 'Value 2 (+)',
                color: 'Joy factors',
                title: 'Stacked Treemap',
                dividedBy: 'Country_code'
            }),
            style: {
                plot: {
                    paddingLeft: '0em',
                    marker: {
                        label: { fontSize: '0.7em' }
                    }
                },
                legend: { maxWidth: '20%' },
            }
        }
    )
];

export default testSteps;