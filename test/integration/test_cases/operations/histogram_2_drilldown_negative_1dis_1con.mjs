import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Year'] },
          y: { attach: ['Positive nums'], range: '0,1.1,%' },
          label: { attach: ['Positive nums'] },
        },
        title: 'Histogram with (-) Nums',
        align: 'none',
        legend: null
      }
  }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { attach: ['Threat'] },
          color: { attach: ['Threat'] }
        },
        title: 'Stacked'
      },
      style: { plot: { marker: { label: {  fontSize: 11,  textAlign: 'center',  position: 'center',  filter: 'lightness(0.8)' } } } }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { detach: ['Threat', 'Positive nums'] },
          y: { attach: ['Negative nums'], range: '0,1.1,%' },
          x: { attach: ['Threat'] },
          label: { detach: ['Positive nums'] },
          label: { attach: ['Negative nums'] },
        },
        title: 'Grouped with (-) Nums',
        split: true,
      },
      style: { plot: { marker: { label: { fontSize: 10, position: 'above', filter: 'lightness(0.6)' } } } }
    }
  )
];

export default testSteps;