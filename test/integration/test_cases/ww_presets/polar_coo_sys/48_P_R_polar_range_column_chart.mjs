import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => chart.animate(
    {
      data: Object.assign(data, {
        filter: record =>
            (record.Country == 'Austria' ||
            record.Country == 'Belgium' ||
            record.Country == 'Bulgaria' ||
            record.Country == 'Cyprus' ||
            record.Country == 'Czechia' ||
            record.Country == 'Denmark' ) &&
            (record["Joy factors"] == 'Love' ||
            record["Joy factors"] == 'Creativity')
          }),
      config: {
        channels: {
          x: {attach: 'Country' },
          y: {attach: ['Joy factors', 'Value 2 (+)'] },
          color: {attach: 'Joy factors'}
        },
        title: 'Polar Range Column Chart',
        coordSystem:'polar',
        legend: null
      },
      style: {
          plot: {
            paddingLeft: '3.8em',
            marker: {
              rectangleSpacing: '0', 
              colorPalette: '#f2b82dFF #00000000 #00000000',
//              label: {position: 'top'}
              },
              yAxis: {
//                title: { color: '#ffffff00' },
//                label: { color: '#ffffff00' },
                  label: { paddingRight: '0em' },
                  ticks: { color: '#ffffff00' }
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