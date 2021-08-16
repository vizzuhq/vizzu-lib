import { data } from '/test/integration/test_data/chart_types_eu.js';
data.filter = record =>
record.Country != 'Germany' &&
record.Country != 'Italy' &&
record.Country != 'Hungary' &&
record.Country != 'Netherlands' &&
record.Country != 'Estonia' &&
record.Country != 'Belgium' &&
record.Country != 'Sweden' &&
record.Country != 'Poland' &&
record.Country != 'Malta' &&
record.Country != 'Romania' &&
record.Country != 'Greece' &&
record.Country != 'Slovakia' &&
record.Country != 'Ireland' &&
record.Country != 'Lithuania' &&
record.Country != 'Croatia' &&
record.Country != 'Slovenia' &&
record.Country != 'Portugal' &&
record.Country != 'Finland' &&
record.Country != 'United Kingdom';

const testSteps = [
    chart => chart.animate(
        {   data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Negative nums'], range: '0,1.5,%' },
                    x: { attach: ['Neg_huge'], range: '0,1.5,%' },
                    color: { attach: ['Country'] },
                    size: { attach: ['Neg_small'] },
                    label: { attach: ['Negative nums'] }
                },
                title: 'Circle Marker Label Positions - Center',
                legend: 'color',
                geometry: 'circle',
            },
            style:
            {
                data: 
                {
                    circleMinRadius: 5,
                    circleMaxRadius: 0.09
                },
                plot:
                {   paddingLeft: 120,                    
                    xAxis:
                    {
                        label: 
                        {
                            position: 'min-edge',
                            side: 'positive'
                        }
                    },
                    yAxis:
                    {
                        label: 
                        {
                            position: 'min-edge',
                            side: 'positive'
                        }
                    },
                    marker:
                    {
                        guides:
                        {
                            color: '#e8e8e8FF',
                            lineWidth: 0.5,
                        },
                        label:
                        {
                            fontWeight: 'bold',
                            position: 'center',
                            filter: 'lightness(0.7)',
                            numberFormat: 'grouped'
                        }
                    },
                    xAxis:
                    {
                        ticks:
                        {
                            color: '#e8e8e8FF',
                            lineWidth: 1
                        }
                    },
                    yAxis:
                    {
                        ticks:
                        {
                            color: '#e8e8e8FF',
                            lineWidth: 1
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Top',
                coordSystem:'polar'
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'top',
                            orientation: 'horizontal',
                            filter: 'lightness(0)'
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;