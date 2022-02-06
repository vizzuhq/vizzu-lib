import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Country', 'Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
                    x: { attach: ['Year'] },
                    color: { attach: ['Country'] },
                },
                title: 'Stacked Area Chart with (-) Nums',
                geometry: 'area'
            }
        }
    )
];

export default testSteps;