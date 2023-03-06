import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => 
  {
    let labelToShow = '';

    chart.on('mouseon', event => {
      labelToShow = event.data.marker.values['Value 5 (+/-)'];
      event.preventDefault();
    })

    chart.on('plot-marker-label-draw', event => {
      if (event.data.text != labelToShow)
        event.preventDefault();
    })

    return chart.animate({
      data: data,
      config: {
        x: 'Year',
        y: 'Value 5 (+/-)',
        label: 'Value 5 (+/-)',
        title: 'Label shown on mouse-on'
      }
    });
  }
];

export default testSteps;