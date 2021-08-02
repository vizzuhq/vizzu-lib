import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
    plot: {
        marker: {
            label: { position: 'top', filter: 'lightness(0)' }
        }
    }
};

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    x: { attach: ['Neg_huge'] },
                    y: { attach: ['$exists', 'Negative nums'], range: '0,1.1,%' },
                    shape: { attach: ['Year'] },
                    label: { attach: ['Year'] }
                },
                title: 'Scatterplot with (-) Nums',
                legend: null,
                geometry: 'circle',
                coordSystem: 'cartesian'
            },
            style: styles
        }
    )
];

export default testSteps;