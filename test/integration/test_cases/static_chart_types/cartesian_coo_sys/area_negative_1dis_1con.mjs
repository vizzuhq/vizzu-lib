import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: ['Year'] },
                    y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
                    label: { attach: ['Value 5 (+/-)'] },
                },
                title: 'Area Chart with (-) Nums',
                align: 'none',
                geometry: 'area'
            }
        }
    )
];

export default testSteps;