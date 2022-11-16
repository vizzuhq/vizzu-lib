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
            record.Country == 'Denmark' 
    }),
      config: {
        channels: {
          x: {set: 'Country' },
          y: {set: ['Joy factors', 'Value 2 (+)'] },
          color: {set: 'Joy factors'},
          label: {set: 'Value 2 (+)' },
        },
        title: 'Percentage Stacked Column Chart',
        align: 'stretch'
      }
    }
  ),
  chart => chart.feature('tooltip',true)
];

export default testSteps;