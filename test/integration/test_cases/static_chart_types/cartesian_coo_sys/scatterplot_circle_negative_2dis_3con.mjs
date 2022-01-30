import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
                    x: { attach: ['Value 6 (+/-)'], range: { min: '0%', max: '110%' } },
                    color: { attach: ['Country'] },
                    size: { attach: ['Value 4 (+/-)'] },
                    label: { attach: ['Value 5 (+/-)'] }
                },
                title: 'Scatterplot with (-) Nums',
                geometry: 'circle'
            }
        }
    )
];

export default testSteps;