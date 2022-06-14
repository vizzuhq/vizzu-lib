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
          x: {attach: ['Joy factors', 'Value 2 (+)'] },
          y: {attach: 'Country' },
          color: {attach: 'Joy factors'},
          label: {attach: 'Value 2 (+)'},
        },
        title: 'Span Bar Chart',
        legend: null
      },
      style: {
          plot: {
            paddingLeft: 70,
            marker: { colorPalette: '#f2b82dFF #00000000 #00000000' },
              yAxis: {
                  label: {
                     paddingRight: 10
                  }
              },
              xAxis: {
                title: { paddingTop: 30 },
                label: { paddingTop: 10 }
              }
          }
      }
    }
  ),
  chart => chart.feature('tooltip',true)
];

export default testSteps;