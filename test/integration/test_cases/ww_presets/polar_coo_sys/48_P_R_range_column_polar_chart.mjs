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
          color: {attach: 'Joy factors'},
          label: {attach: 'Value 2 (+)'},
        },
        title: 'Range Column Polar Chart',
        coordSystem:'polar',
        legend: null
      },
      style: {
          plot: {
            paddingLeft: 50,
            marker: {
              rectangleSpacing: '0', 
              colorPalette: '#f2b82dFF #00000000 #00000000' 
              },
              yAxis: {
                title: { color: '#ffffff00' },
                label: { color: '#ffffff00' },
                ticks: { color: '#ffffff00' }
              },
              xAxis: {
                  label: { paddingTop: 10 }
              }
          }
      }
    }
  ),
  chart => chart.feature('tooltip',true)
];

export default testSteps;