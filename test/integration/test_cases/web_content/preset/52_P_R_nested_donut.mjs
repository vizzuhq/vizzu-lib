import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: Object.assign(data, {
                filter: record =>
                    record.Country == 'Austria' ||
                    record.Country == 'Belgium' ||
                    record.Country == 'Bulgaria' 
            }),
            config: chart.constructor.presets.nestedDonut({
                angle:'Value 2 (+)',
                stackedBy:'Joy factors',
                radius:'Country',
                title: 'Nested Donut Chart'
              }),
            style: {
                plot: {
                    paddingLeft: '0em',
                    paddingRight: '12.42em',
                    marker: {
                        rectangleSpacing:'0',
                        borderWidth: 1,
                        borderOpacity: 0
                        },
                    yAxis: {
                        color: '#ffffff00',
                        title: { color: '#ffffff00' },
                        label: { color: '#ffffff00' },
                        ticks: { color: '#ffffff00' }
                    },
                    xAxis: {
                        title: { color: '#ffffff00' },
                        interlacing: { color: '#ffffff00' },
                        label: { color: '#ffffff00' }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;