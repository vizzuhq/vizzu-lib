import { data } from '../../../test_data/music_industry_history_1.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: Object.assign(data, {
                filter: record =>
                    record.Format == 'DVD' ||
                    record.Format == 'Other' ||
                    record.Format == 'Tapes' ||
                    record.Format == 'Download' ||
                    record.Format == 'Streaming' ||
                    record.Format == 'Cassette' ||
                    record.Format == 'Vinyl' ||
                    record.Format == 'CD'
            }),

            config: {
                channels: {
                    x: { set: ['Revenue [m$]', 'Format'], range: { min: '-1%', max: '110%' } },
                    y: { set: ['Year'] },
                    color: { set: 'Format' }
                },
                title: 'Violin',
                geometry: 'area',
                align: 'center',
                orientation: 'vertical',
                split: true
            }
        },
        {
                 delay: 0,
         }
        ),
        chart => chart.animate({
        config: {
            channels: {
                x: { set: [/*'Revenue [m$]',*/ 'Format'] },
                y: { set: ['Year'] },
                color: { set: 'Format' },
                size: { set: 'Revenue [m$]' }
            },
            title: 'Violin',
            geometry: 'circle',
            align: 'center',
            orientation: 'vertical',
            split: true
        }
    },
    {
             delay: 1,
     }
    ),
    
        chart => chart.feature('tooltip',true)
    ];



export default testSteps;