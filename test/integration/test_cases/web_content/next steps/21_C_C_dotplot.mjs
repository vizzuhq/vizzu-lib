import { data } from '../../../test_data/IMDB_data.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: Object.assign(data, {
                filter: record =>
                    record.Year == '1974' ||
                    record.Year == '1975' ||
                    record.Year == '1976' ||
                    record.Year == '1977' ||
                    record.Year == '1978' ||
                    record.Year == '1979' ||
                    record.Year == '1980' ||
                    record.Year == '1981' ||
                    record.Year == '1982' ||
                    record.Year == '1983' ||
                    record.Year == '1984' ||
                    record.Year == '1985' ||
                    record.Year == '1986' ||
                    record.Year == '1987' ||
                    record.Year == '1988' ||
                    record.Year == '1989' ||
                    record.Year == '1990' 
            }),
            config:
            {
                channels:
                {
                    x: { set: 'Year' },
                    y: { set: 'Index', range: { max: '110%' } }
                },
                title: 'Dot plot',
                align: 'none',
                geometry: 'circle'
            },
            style:
            {
                plot: {
                    paddingLeft: '8em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                    },
                xAxis: {
                    title: { paddingTop: '2.4em' },
                    label: { paddingTop: '0.8em' }
                    }
                }
            }
        }
    ),

    chart => chart.animate({
        config: {
                channels:
                {
                    x: { set: 'Year' },
                    y: { set: 'IMDb Rating', range: { max: '110%' } },
                    noop: { set: 'Index' }
                },
                title: 'Dot plot / + Continuous(?), Distribution(?)',
                align: 'none',
                orientation: 'vertical',
                split: true,
                geometry: 'circle'
        }
    },
    {
        delay: 1.5,
        easing: 'cubic-bezier(0.65,0,0.65,1)',
         geometry: { 
             delay: 0.5, 
             duration: 0.5, 
//                easing: 'linear' 
         },
         x: {
             delay: 0,
             duration: 0.8,
//                easing: 'ease-out'
         }, 
         y: {
             delay: 0.2,
             duration: 0.8,
//                easing: 'cubic-bezier(65,0,65,1)'
         }
     }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;