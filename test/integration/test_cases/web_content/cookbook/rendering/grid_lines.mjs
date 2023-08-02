import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
	async chart => {
		await import('https://unpkg.com/tinycolor2@1.6.0/dist/tinycolor-min.js');

		chart.on('plot-axis-interlacing-draw', event => {

			let ctx = event.renderingContext;
			let rect = event.data.rect;

			ctx.strokeStyle = '#cccccc';

			ctx.globalAlpha = tinycolor(ctx.fillStyle).getAlpha(); // support fade-in
	  
			ctx.lineWidth = 1;

			ctx.beginPath();
			ctx.moveTo(rect.pos.x, rect.pos.y);
			ctx.lineTo(rect.pos.x + rect.size.x, rect.pos.y);
			ctx.stroke();

			ctx.beginPath();
			ctx.moveTo(rect.pos.x, rect.pos.y + rect.size.y);
			ctx.lineTo(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y);
			ctx.stroke();

			ctx.globalAlpha = 1;

			event.preventDefault();
		});

		return chart.animate({
			data: data,
			config: {
				x: 'Joy factors',
				y: 'Value 2 (+)',
				color: 'Joy factors',
				title: 'Axis grid lines'
			}
		})
	}
];

export default testSteps;