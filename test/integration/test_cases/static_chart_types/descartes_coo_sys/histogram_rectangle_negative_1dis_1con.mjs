import { data } from '/test/integration/test_data/chart_types_eu.js';

  let styles = { plot: { marker:{ label:{ position: 'top', filter: 'lightness(0)' }} }};

  const testSteps = [
    chart => chart.animate(
      {
        data: data,
        descriptor: {
          channels: {
            x: {attach: ['Year']},
            y: {attach: ['Negative nums'], range: '0,1.1,%'},
            label: { attach: ['Negative nums']}
          },
          title: 'Histogram with (-) Nums',
          align: 'none',
          legend: null
        },
        style: styles
      }
    )
];

export default testSteps;