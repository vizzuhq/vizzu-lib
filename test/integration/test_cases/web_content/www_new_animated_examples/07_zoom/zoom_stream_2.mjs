import { data } from '../../../../test_data/music_industry_history_1.mjs';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                (
                    record.Format == 'Tapes'
                ) &&
                (
                    record['Year'] == '1973' ||
                    record['Year'] == '1974' ||
                    record['Year'] == '1975' ||
                    record['Year'] == '1976' ||
                    record['Year'] == '1977' ||
                    record['Year'] == '1978' ||
                    record['Year'] == '1979' ||
                    record['Year'] == '1980' ||
                    record['Year'] == '1981' 
                )
        }),
        config: {
            channels: {
                x: ['Year'],
                y: ['Revenue [m$]', 'Format'],
                color: 'Format'
            },
            title:'Title',
            geometry: 'area',
            align: 'center',
//            split: true
        }
    }
),

    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                (
                    record.Format == 'Tapes' ||
                    record.Format == 'Vinyl'
                ) &&
                (
                    record['Year'] == '1973' ||
                    record['Year'] == '1974' ||
                    record['Year'] == '1975' ||
                    record['Year'] == '1976' ||
                    record['Year'] == '1977' ||
                    record['Year'] == '1978' ||
                    record['Year'] == '1979' ||
                    record['Year'] == '1980' ||
                    record['Year'] == '1981' ||
                    record['Year'] == '1982' ||
                    record['Year'] == '1983' ||
                    record['Year'] == '1984' ||
                    record['Year'] == '1985' ||
                    record['Year'] == '1986' ||
                    record['Year'] == '1987' ||
                    record['Year'] == '1988' ||
                    record['Year'] == '1989' 
                )
        }),
            config: {
                title:'Title'
            }    
    }
),

    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                (
                    record.Format == 'Tapes' ||
                    record.Format == 'Cassette' ||
                    record.Format == 'Vinyl'
                ) &&
                (
                    record['Year'] == '1973' ||
                    record['Year'] == '1974' ||
                    record['Year'] == '1975' ||
                    record['Year'] == '1976' ||
                    record['Year'] == '1977' ||
                    record['Year'] == '1978' ||
                    record['Year'] == '1979' ||
                    record['Year'] == '1980' ||
                    record['Year'] == '1981' ||
                    record['Year'] == '1982' ||
                    record['Year'] == '1983' ||
                    record['Year'] == '1984' ||
                    record['Year'] == '1985' ||
                    record['Year'] == '1986' ||
                    record['Year'] == '1987' ||
                    record['Year'] == '1988' ||
                    record['Year'] == '1989' ||                     
                    record['Year'] == '1990' ||
                    record['Year'] == '1991' ||
                    record['Year'] == '1992' ||
                    record['Year'] == '1993' ||
                    record['Year'] == '1994' ||
                    record['Year'] == '1995' ||
                    record['Year'] == '1996' ||
                    record['Year'] == '1997' ||
                    record['Year'] == '1998' ||
                    record['Year'] == '1999' 
                )
        }),
            config: {
                title:'Title'
            }    
    }
),
/*
    chart => chart.animate({
        data: Object.assign(data, {
         filter: record =>
            record.Format == 'Tapes' ||
            record.Format == 'Cassette' ||
            record.Format == 'Vinyl' ||
            record.Format == 'CD'
}),
            config: {
                title:'Title'
            }    
    }
),
*/
    chart => chart.animate({
        data: Object.assign(data, {
        filter: record =>
            record.Format == 'DVD' ||
            record.Format == 'Other' ||
            record.Format == 'Tapes' ||
            record.Format == 'Cassette' ||
            record.Format == 'Vinyl' ||
            record.Format == 'CD'
}),
            config: {
                title:'Title'
            }    
    }
),

chart => chart.animate({
    data: Object.assign(data, {
    filter: record =>
        record.Format == 'DVD' ||
        record.Format == 'Other' ||
        record.Format == 'Tapes' ||
        record.Format == 'Download' ||
        record.Format == 'Cassette' ||
        record.Format == 'Vinyl' ||
        record.Format == 'CD'
}),
        config: {
            title:'Title'
        }    
}
),
chart => chart.animate({
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
            title:'Title'
        }    
}
),
chart => chart.feature('tooltip',true)
];

export default testSteps;