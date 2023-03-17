import { data_1974_1990 } from '../../../../test_data/IMDB_data.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data_1974_1990,
            config: {
                channels: {
                    x: 'Year',
                    y: { set: 'Index', range: { max: '105%' } }
                },
                title:'Title',
                geometry: 'circle'
            }
        }
    ),

    chart => chart.animate({
        config: {
                channels: {
                    y: { set: 'IMDb Rating', range: { max: '105%' } },
                    noop: 'Index'
                },
                title:'Title',
                orientation: 'vertical',
                split: true
        }
    }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;