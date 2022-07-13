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
                    record.Country == 'Hungary'
            }),
            config: chart.constructor.presets.polarLine({
                angle:'Year',
                radius:'Value 2 (+)',
                dividedBy:'Country',
                title: 'Polar Line Chart'
              }),
            style: {
                plot: {
                    paddingLeft: '0em',
                    paddingRight: '12.42em',
                    yAxis: {
                        label: {
                           paddingRight: '0em'
                        }
                    },
                    xAxis: {
                        label: {
                           paddingTop: '0.8em'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;