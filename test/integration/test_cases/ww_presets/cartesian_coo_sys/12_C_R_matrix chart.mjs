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
                    x: { attach: 'Year' },
                    y: { attach: 'Index', range: { max: '100%' } }
                },
                title: 'Matrix Chart',
                align: 'none'
            },
            style:
            {
                plot: {
                    paddingLeft: '8em',
                    marker: { borderOpacity: 0, rectangleSpacing: 0 },
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
    chart => chart.feature('tooltip',true)
];

export default testSteps;