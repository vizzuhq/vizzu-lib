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
      config: {
        channels: {
          x: {attach: 'Country' },
          y: {attach: ['Joy factors', 'Value 2 (+)'] },
          color: {attach: 'Joy factors'}
        },
        title: 'Polar Area Chart',
        coordSystem: 'polar',
      },
      style: {
          plot: {
              paddingLeft: '0em',
              paddingRight: '12.42em',
              marker: {
                rectangleSpacing: '0'
              },
              yAxis: {
                  color: '#00000000',
                  label: { paddingRight: '0em' },
                  ticks: { color: '#ffffff00' },
              },
              xAxis: {
                  label: { paddingTop: '0.8em' }
              }
          }
      }
    }
  ),
  chart => chart.feature('tooltip',true)
];

export default testSteps;