import { data } from '/test/integration/test_data/chart_types_eu.js';

let snapshot;

const testSteps = [

  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          y: { attach: ['Joy factors', 'Value 2 (+)']},
          x: { attach: ['Country_code']},
          color: {attach: ['Joy factors']}
        },
        title: 'Column Chart Stacked'
      }
    }
  ),
  chart => {
    snapshot = chart.store();
    return chart;
  },
  chart => chart.animate({ 
    align: 'stretch' 
  }),
  chart => chart.animate({ 
    align: 'min' 
  }),
  chart => chart.animate({ 
    channels: {
      x: { attach: 'Value 2 (+)' }
    }
  }),
  chart => chart.animate({ 
      y:  'Value 2 (+)',
      x: [ 'Country_code', 'Joy factors' ]
  }),
  chart => chart.animate({ 
style: {
  'plot.xAxis.label.fontSize': '70%'
}
}),
chart => chart.animate(snapshot)

];

export default testSteps;