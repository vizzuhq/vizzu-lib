import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Timeseries'] },
          y: { attach: ['Categ. Parent', 'Values 1'], range: '0,1.1,%' },
          color: { attach: ['Categ. Parent'] },
          label: { attach: ['Values 1'] }
        },
        title: 'Operations: Rectangle - Merged.',
        legend: "color",
        geometry: 'rectangle'
      },
      style: { plot: { marker: { borderWidth: 0, borderOpacity: 1 } } }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Operations: Rectangle - Splitted.',
        split: true,
      },
      style: { plot: { marker: { label: { position: 'top', filter: 'lightness(0.1)' } } } }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Operations: Rectangle - Merged.',
        split: false
      },
      style: { plot: { marker: { label: { position: 'center', filter: 'lightness(0.8)' } } } }
    }
  )
];

export default testSteps;