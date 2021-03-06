import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          color: { attach: ['Joy factors'] },
          size: { attach: ['Country_code', 'Value 2 (+)'] },
          label: { attach: ['Country_code'] },
          lightness: { attach: ['Value 4 (+/-)'] }
        },
        title: 'Bubble Chart',
        geometry: 'circle'
      },
      style: 
      {
          plot: 
          {
              marker: 
              {
                  label: 
                  {
                      fontSize: 9
                  }
              }
          }
      }
    }
  )
];

export default testSteps;