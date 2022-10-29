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
            config: chart.constructor.presets.radialStackedBar({
                angle: 'Value 2 (+)',
                radius: 'Country',
                stackedBy: 'Joy factors',
                title: 'Radial Stacked Bar Chart'
            }),
            style: {
                plot: {
                    paddingLeft: '0em',
                    paddingRight: '12.42em',
                    yAxis: {
                        color: '#ffffff00',
                        title: { color: '#ffffff00' },
                        ticks: { color: '#ffffff00' },
                        label: { paddingRight: '0.8em' }
                    },
                    xAxis: {
                        title: { color: '#ffffff00' },
                        interlacing: { color: '#ffffff00' },
                        label: { color: '#ffffff00' }
                    }
                }
            }
        }
    )
];

export default testSteps;