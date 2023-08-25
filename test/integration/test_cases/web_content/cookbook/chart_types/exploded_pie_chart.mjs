import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => {
        let dx = 0;
        let dy = 0;

        chart.on('plot-marker-label-draw', event => {
            let ctx = event.renderingContext;
            if (dx !== 0 || dy !== 0) {
                ctx.translate(-dx, -dy);
                dx = 0;
                dy = 0;
            }
        });

        chart.on('plot-marker-draw', event => {
            let ctx = event.renderingContext;
            let rect = event.detail.rect;
            let angle = (rect.pos.x + rect.size.x / 2) * 2 * 3.14;
            const d = 10;
            if (dx !== 0 || dy !== 0) {
                ctx.translate(-dx, -dy);
                dx = 0;
                dy = 0;
            }
            dx = + d * Math.sin(angle);
            dy = - d * Math.cos(angle);
            ctx.translate(dx, dy);
        });

        return chart.animate({
            data: data,
            config: {
                channels: {
                    x: ['Joy factors', 'Value 2 (+)'],
                    color: 'Joy factors',
                    label: 'Value 2 (+)'
                },
                title: 'Exploded Pie Chart',
                coordSystem: 'polar'
            }
        })
    }
];

export default testSteps;