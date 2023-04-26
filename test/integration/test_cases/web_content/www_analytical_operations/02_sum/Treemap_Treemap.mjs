import { data } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- add the Dimension whitch is on Label channel to the Size channel too (stack)`
;

const testSteps = [
  chart => chart.animate({
    data: data,
    config: {
      channels: {
        size: 'Value 2 (+)',
        /* The noop channel (no operation) splits the markers as all the other channels
        but will have no effect on the markers’ appearance. */
        noop: 'Country_code',
        color: 'Joy factors',
        label: 'Country_code'
      },
      title:'Title'
    }
  }),

  chart => chart.animate({
    config: {
      /* With a dimension on the size channel Vizzu will stack the elements
      by the categories on the other channels on charts without coordinates.
      Here the Country code dimension is used to stack the bubbles
      by the dimension on the color channel. */
      channels: {
        size: ['Country_code', 'Value 2 (+)'] 
      },
      title:'Title'
    }
  })
];

export default testSteps;