import { data } from '../../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                record.Country == 'Austria' ||
                record.Country == 'Belgium' ||
                record.Country == 'Bulgaria' ||
                record.Country == 'Cyprus' ||
                record.Country == 'Czechia' ||
                record.Country == 'Denmark' ||
                record.Country == 'Estonia' ||
                record.Country == 'Greece' ||
                record.Country == 'Germany' ||
                record.Country == 'Spain' ||
                record.Country == 'Finland' ||
                record.Country == 'France' ||
                record.Country == 'Croatia' ||
                record.Country == 'Hungary'
        }),
        config:
        {
            channels:
            {
                y: { set: 'Value 3 (+)' },
                x: { set: 'Value 2 (+)' },
                color: { set: 'Country' },
                noop: { set: 'Year' }
            },
            title: 'Scatter plot',
            geometry: 'circle',
            legend: null
        },
        style: {
            plot: {
                paddingLeft: 100,
                yAxis: {
                    label: {
                       paddingRight: 10,
                        fontSize: 13
                    }
                }/*,
                marker: {
                    guides: {
                        color: null,
                        lineWidth: 0
                    }
                }*/
            }
        }
    }
),

chart => chart.animate(
    {
        config: {
            channels:
            {
                y: { set: null, range: { min: '0%', max: '800%' } },
                x: { set: 'Value 2 (+)', range: { min: '0%', max: '200%' }  },
                color: { set: null },
                noop: { set: null },
                size: { set: ['Year', 'Country'] }
            },
            title: 'Scatter plot (stacked).',
            align: 'center'
        }
    },
    {
         coordSystem: {
             delay: 0,
             duration: 1,
         },
         geometry: { 
             delay: 0, 
             duration: 0, 
//                easing: 'linear' 
         },
         x: {
             delay: 0,
             duration: 1,
//                easing: 'ease-out'
         }, 
         y: {
             delay: 0,
             duration: 1,
//                easing: 'cubic-bezier(65,0,65,1)'
         }
     }
),

chart => chart.animate(
    {
        config: {
            channels:
            {
                noop: { set: null },
                size: { set: ['Year', 'Country', 'Joy factors'] }
            },
            title: 'Scatter plot (stacked).'
        }
    },
    {
             duration: 0
     }
),


chart => chart.animate(
    {
        config: {
            channels:
            {
                noop: { set: null },
                size: { set: 'Joy factors' }
            },
            title: 'Scatter plot (stacked).'
        }
    },
    {
             duration: 0
     }
),



chart => chart.animate(
    {
        config: {
            channels:
            {
                y: { set: 'Value 1 (+)', range: { min: '-10%', max: '110%' } },
                x: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' }  },
                color: { set: 'Joy factors' },
                size: { set: null  }
            },
            title: 'Scatter plot'
        }
    }
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: null },                
            noop: { set: 'Joy factors' },
            size: { set: 'Value 1 (+)' },
            lightness: { set: 'Value 2 (+)' },
        },
        title: 'Bubble Chart',
        geometry: 'circle',
        orientation: 'horizontal'
    }
},
   {
    easing: 'cubic-bezier(0.65,0,0.65,1)',
        coordSystem: {
            delay: 0,
            duration: 1,
        },
        geometry: { 
            delay: 0, 
            duration: 1, 
//                easing: 'linear' 
        },
        x: {
            delay: 0,
            duration: 1,
//                easing: 'ease-out'
        }, 
        y: {
            delay: 0,
            duration: 1,
//                easing: 'cubic-bezier(65,0,65,1)'
        }
    }
)
];

export default testSteps;