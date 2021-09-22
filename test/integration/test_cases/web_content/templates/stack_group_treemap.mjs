
import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
  chart => chart.animate({
    data: data,
    config: {
      channels: {
        size: { set: ['Value 2 (+)'] },
        noop: { set: ['Country_code'] }, // noop (no operation) channel ...???
        color: { set: ['Joy factors'] },
        label: { set: ['Country_code'] }
      },
      title: 'Treemap Comparision.',
      geometry: 'rectangle'
    }
  }),
  chart => chart.animate({
    config: {
      channels: {
        size: { set: ['Value 2 (+)', 'Country_code'] } // Put dimension data series on 'size' channel to stack by other dimension which is on 'color' channel. 
      },
      title: 'Treemap Stacked.'
    }
  })
];

export default testSteps;