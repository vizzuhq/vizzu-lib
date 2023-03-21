import { data } from '../../../../test_data/music_industry_history_1.mjs';

const testSteps = [
    chart => {
        chart.on('plot-axis-label-draw', event => {
            let year = parseFloat(event.data.text);
            if (!isNaN(year) && year % 5 != 0)
                event.preventDefault();
        });
            return chart
    },

    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                record['Year'] > 1972
        }),
        config: {
            channels: {
                x: 'Year',
                y: ['Revenue [m$]', 'Format'],
                color: 'Format'
            },
            title:'Title',
            geometry: 'area',
            align: 'center',
        }
    }),

    chart => chart.animate({
        config: {
            title:'Title',
            align: 'min',
            split: true
        }
    }),

    chart => chart.animate({
            config: {            
                channels: {
                x: ['Format', 'Year'],
                y: 'Revenue [m$]'
            },
                title:'Title',
                split: false
        }    
    }),
    chart => chart.feature('tooltip',true)
];

export default testSteps;