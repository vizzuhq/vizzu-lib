import { data } from '../../../test_data/music_industry_history_1.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: chart.constructor.presets.verticalViolin({
                x: 'Revenue [m$]',
                y: 'Year',
                splittedBy: 'Format',
                title: 'Vertical Violin Graph'
              }),
            style: {
                plot: {
                    xAxis: {
                        interlacing: { color: '#ffffff00' }
                    }
                }
            }
        })
];

export default testSteps;