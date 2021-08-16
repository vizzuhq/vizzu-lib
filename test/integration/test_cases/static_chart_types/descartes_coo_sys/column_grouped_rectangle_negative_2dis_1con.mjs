import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles =
{
    data: { columnMaxPadding: 0.13 },
    plot: {
        marker: { label: { fontSize: 7, position: 'top', filter: 'lightness(0)' } },
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
                    y: { attach: ['Negative nums'], range: '0,1.1,%' },
                    x: { attach: ['Threat', 'Country'] },
                    color: { attach: ['Threat'] },
                    label: { attach: ['Negative nums'] }
                },
                title: 'Grouped Column Chart with (-) Nums',
                legend: null

            },
            style:
            {
                data:
                { columnMaxPadding: 0.13 },
                plot:
                {
                    marker:
                    { label:
                        {
                            fontSize: 7,
                            position: 'top',
                            orientation:'vertical',
                            angle:3.14*-1,
                            filter: 'lightness(0)'
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;