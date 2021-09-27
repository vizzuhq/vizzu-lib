
import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
  chart => chart.animate({
    data: data,
    config: {
      channels: {
        size: { set: ['Value 2 (+)'] },
        /* The noop channel splits the markers as all the other channels
        but will have no effect on the markers’ appearance. */
        noop: { set: ['Country_code'] },
        color: { set: ['Joy factors'] },
        label: { set: ['Country_code'] }
      },
      title: 'Treemap.',
      geometry: 'rectangle'
    }
  }),
  chart => chart.animate({
    config: {
      /* With a dimension on the size channel Vizzu will stack the elements
      by the categories on the other channels on charts without coordinates.
      Here the Country code dimension is used to stack the bubbles
      by the dimension on the color channel. */
      channels: {
        size: { set: ['Value 2 (+)', 'Country_code'] } 
      },
      title: 'Stacked Treemap.'
    }
  })
];

export default testSteps;