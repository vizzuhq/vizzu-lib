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
                title: '1',
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
                        title: 
                        {
                            position: 'min-edge',
                            side: 'positive',
                            vposition: 'middle',
                            vside:'upon',
                            orientation:'horizontal'
                        },
                        label: 
                        {
                            position: 'min-edge',
                            side: 'negative'
                        },
                        ticks:
                        {
                            color: '#e8e8e8FF',
                            lineWidth: 1
                        }
                    },
                    yAxis:
                    {
                        title: 
                        {
                            paddingBottom: 15,
                            position: 'min-edge',
                            side: 'upon',
                            vposition: 'end',
                            vside:'negative',
                            orientation:'horizontal'
                        },
                        label: 
                        {
                            position: 'min-edge',
                            side: 'negative'
                        },                        
                        ticks:
                        {
                            color: '#e8e8e8FF',
                            lineWidth: 1
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
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {   
            descriptor:
            {
                title: '2'
            },
            style:
            {
                plot:
                {   paddingLeft: 120,                    
                    xAxis:
                    {
                        title: 
                        {
                            paddingTop: 20,
                            position: 'min-edge',
                            side: 'positive',
                            vposition: 'middle',
                            vside:'upon',
                            orientation:'horizontal'
                        },
                        label: 
                        {
                            paddingTop: 5,
                            position: 'min-edge',
                            side: 'negative'
                        }
                    },
                    yAxis:
                    {
                        title: 
                        {
                            paddingBottom: 15,
                            position: 'axis',
                            side: 'upon',
                            vposition: 'end',
                            vside:'negative',
                            orientation:'horizontal'
                        },
                        label: 
                        {
                            position: 'min-edge',
                            side: 'negative'
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
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: '3',
                coordSystem:'polar'
            },
            style: {
                plot: {
                    xAxis:
                    {
                        title: 
                        {
                            position: 'max-edge',
                            side: 'positive',
                            vposition: 'middle',
                            vside:'upon',
                            orientation:'horizontal'
                        },
                        label: 
                        {
                            position: 'max-edge',
                            side: 'positive'
                        }
                    },
                    yAxis:
                    {
                        title: 
                        {
                            position: 'min-edge',
                            side: 'upon',
                            vposition: 'end',
                            vside:'negative',
                            orientation:'horizontal'
                        },
                        label: 
                        {
                            position: 'min-edge',
                            side: 'negative'
                        }
                    }/*,
                    marker: {
                        label: {
                            position: 'top',
                            orientation: 'horizontal',
                            filter: 'lightness(0)'
                        }
                    }*/
                }
            }
        }
    )
];

export default testSteps;