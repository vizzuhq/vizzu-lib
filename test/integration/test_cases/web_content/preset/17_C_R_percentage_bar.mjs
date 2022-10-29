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
      config: chart.constructor.presets.percentageBar({
        x: 'Value 2 (+)',
        y: 'Country',
        stackedBy: 'Joy factors',
        title: 'Percentage Bar Chart'
      }),
      style: {
        plot: {
          paddingLeft: '1.2em',
          yAxis: {
            label: {
              paddingRight: '0.8em'
            }
          },
          xAxis: {
            title: { paddingTop: '2.4em' },
            label: { paddingTop: '0.8em' }
          }
        }
      }
    }
  )
];

export default testSteps;