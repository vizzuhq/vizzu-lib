import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => 
  {
    let clickHandler = event => 
    {
      let filter = event.target && event.target.tagName === 'plot-marker'
        ? record => record.Year === event.target.categories.Year
        : null;

      chart.animate({ data: { filter } });

      event.preventDefault();
    }

    chart.on('click', clickHandler);

    return chart.animate({
      data: data,
      config: {
        x: 'Year',
        y: 'Value 2 (+)',
        label: 'Value 2 (+)',
        title: 'Filter on click'
      }
    })
  }
];

export default testSteps;