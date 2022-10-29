import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => chart.animate(
    {
      data: Object.assign(data, {
        filter: record =>
            record.Country == 'Austria' ||
            record.Country == 'Belgium' ||
            record.Country == 'Bulgaria' ||
            record.Country == 'Cyprus' ||
            record.Country == 'Czechia' ||
//               record.Country == 'Germany' ||
            record.Country == 'Denmark' 
//               record.Country == 'Estonia' ||
//               record.Country == 'Greece' ||
//               record.Country == 'Spain' ||
//               record.Country == 'Finland' ||
//               record.Country == 'France' ||
//               record.Country == 'Croatia' ||
//              record.Country == 'Hungary'
    }),
      config: {
        channels: {
          x: {attach: ['Joy factors', 'Value 2 (+)'] },
          y: {attach: 'Country' },
          color: {attach: 'Joy factors'}
        },
        title: 'Percentage Stacked Bar Chart',
        align: 'stretch'
      }
    }
  ),
  chart => chart.feature('tooltip',true)
];

export default testSteps;