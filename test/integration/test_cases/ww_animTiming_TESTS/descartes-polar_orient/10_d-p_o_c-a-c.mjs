import { data } from '../../../test_data/chart_types_eu.mjs';


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
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Value 2 (+)'] },
                noop: { set: ['Country'] },
                color: { set: ['Country'] },
            },
            title: 'Dotplot',
            geometry: 'circle',
            legend: null
        }
    }),


    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Year', 'Value 2 (+)'] },
                y: { set: ['Country'] },
                color: { set: ['Country'] },
            },
            title: 'Radial Area Chart Vertical',
            geometry: 'area',
            coordSystem:'polar',
            orientation: 'horizontal',
            split: false,
        }
    }
    ),

    
    chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Value 4 (+/-)'] },
            y: { set: ['Value 2 (+)'] },
            noop: { set: ['Year'] },
            color: { set: ['Country'] },
        },
        title: 'Dotplot',
        geometry: 'circle',
        coordSystem:'cartesian',
        legend: null
    } 
}

)];

export default testSteps;