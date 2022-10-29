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
                    record.Country == 'Germany' ||
                    record.Country == 'Hungary'
            }),
            config: chart.constructor.presets.splittedArea({
                x: 'Year',
                y: 'Value 2 (+)',
                splittedBy: 'Country',
                title: 'Splitted Area Chart'
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
                        label: {
                            paddingTop: '0.8em'
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;