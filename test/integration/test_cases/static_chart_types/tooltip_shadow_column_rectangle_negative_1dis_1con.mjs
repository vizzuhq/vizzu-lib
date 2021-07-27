import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
    data: { columnMaxPadding: 0.15 },
    plot: {
        paddingLeft: 100,
        marker: {
            borderWidth: 0,
        }
    }
};

export default testSteps;
chart.on('plot-marker-draw', event => {
    let ctx = event.renderingContext;
    if (
        event.data.rect.pos.x < 0.037251 + 0.01
     && event.data.rect.pos.x > 0.037251 - 0.01)
    {

        ctx.shadowColor = '#00000060';
        ctx.shadowBlur = 3;
        ctx.shadowOffsetX = 1;
        ctx.shadowOffsetY = 1;
    } else {

        ctx.shadowColor = '#00000000';
        ctx.shadowBlur = 0;
        ctx.shadowOffsetX = 0;
        ctx.shadowOffsetY = 0;
    }
})

chart.on('plot-axis-label-draw', event => {
    let ctx = event.renderingContext;
    ctx.shadowColor = '#00000000';
    ctx.shadowBlur = 0;
})
const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: [/*'Country', 'Year',*/ 'Negative nums'], range: '0,1.1,%' },
                    x: { attach: ['Country_code'] },
//                    color: { attach: ['Country'] },
                    lightness: { attach: ['Negative nums'] }
                },
                title: 'Stacked Column Chart with (-) Numbers',
                legend: 'lightness'
            },
            style: styles
        }
    )
];

export default testSteps;