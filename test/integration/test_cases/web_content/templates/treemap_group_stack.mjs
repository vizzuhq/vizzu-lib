import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          size: { attach: ['Value 2 (+)'] },
          noop: { attach: ['Country_code'] },
          color: { attach: ['Joy factors'] },
          label: { attach: ['Country_code'] }
        },
        title: 'Treemap Comparision.',
        geometry: 'rectangle'
      }

    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { attach: ['Country_code'] }
        },
        title: 'Treemap Stacked.'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { detach: ['Country_code'] }
        },
        title: 'Treemap Comparision.'
      }
    }
  )
];

export default testSteps;