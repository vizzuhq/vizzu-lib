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
          record.Country == 'Denmark' ||
          record.Country == 'Estonia'
      }),
      config: chart.constructor.presets.polarStackedColumn({
        angle: 'Country',
        radius: 'Value 2 (+)',
        stackedBy: 'Joy factors',
        title: 'Polar Stacked Column Chart'
      })
    }
  )
];

export default testSteps;