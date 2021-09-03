import { data } from '/test/integration/test_data/chart_types_eu.js';

  const testSteps = [
    chart => chart.animate(
      {
        data: data,
        config: {
          channels: {
            x: {attach: ['Year']},
            y: {attach: ['Value 5 (+/-)'], range: '0,1.1,%'},
            label: { attach: ['Value 5 (+/-)']}
          },
          title: 'Histogram',
          align: 'none'
        }
      }
    ),
    chart => chart.animate(
      {
        style: {
          'plot.marker.rectangleSpacing': null
        }
      }
    )
];

export default testSteps;