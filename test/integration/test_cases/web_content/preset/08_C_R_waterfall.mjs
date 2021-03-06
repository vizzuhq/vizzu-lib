import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: Object.assign(data, {
                filter: record => record.Country == 'Belgium'
            }),
            config: chart.constructor.presets.waterfall({
                x: 'Year',
                y: 'Value 5 (+/-)',
                title: 'Waterfall Chart'
            }),
            style:
            {
                plot: {
                    paddingLeft: '8em',
                    marker: {
                        colorGradient:
                            [
                                '#ff001b 0',
                                '#ff001b 0.5',
                                '#7e79e8 0.5',
                                '#7e79e8 1'
                            ].join(),
                        label: {
                            position: 'top'
                        }
                    },
                    yAxis: {
                        label: {
                            paddingRight: '0.8em'
                        }
                    },
                    xAxis: {
                        label: { paddingTop: '0.8em' }
                    }
                }
            }
        }
    )
];

export default testSteps;