import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
  chart => chart.animate({
    data: data,
    config: {
      channels: {
        color: { set: ['Joy factors'] },
        size: { set: ['Country_code', 'Value 2 (+)'] },
        label: { set: ['Country_code'] }
      },
      title: 'Stacked Bubble Chart',
      geometry: 'circle'
    },
    style: {
      plot: {
        marker: {
          label: {
            fontSize: 9
          }
        }
      }
    }
  })
];

export default testSteps;