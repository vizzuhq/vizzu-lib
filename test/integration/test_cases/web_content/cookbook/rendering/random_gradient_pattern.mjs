import { data_14 } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = 
[
	async chart => 
	{
		let tinycolor = (await import('https://esm.sh/tinycolor2')).default;

		var seed = 1;
		
		let rand = () => {
			var x = Math.sin(seed++) * 10000;
			return x - Math.floor(x);
		}

		chart.on('draw-begin', event => {
			seed = 1;
		});

		chart.on('plot-marker-draw', event => 
		{
			let ctx = event.renderingContext;
			let rect = event.data.rect;

			let color = ctx.fillStyle; 
			if (!tinycolor(color).isValid()) return;
			let alpha = tinycolor(color).getAlpha();

			for (let x = rect.pos.x; x <= rect.pos.x + rect.size.x; x++)
			{
				let y = rect.pos.y;
				let yLen = rect.size.y * ((2 + rand()) / 3);

				let grad = ctx.createLinearGradient(0, y, 0, y + yLen);
				grad.addColorStop(0, tinycolor(color));
				grad.addColorStop(0.5, tinycolor(color).desaturate(25).darken(25).setAlpha(alpha));
				grad.addColorStop(1, tinycolor(color).desaturate(100).setAlpha(0));
				ctx.fillStyle = grad;
				ctx.fillRect(x, y, 1, yLen);
			}

			event.preventDefault();
		});

		return chart.animate({
			data: data_14,
			config: {
				x: {
					set: 'Country_code',
					axis: false
				},
				y: { 
					set: 'Value 3 (+)',
//					range: { min: 600 },
				},
				color: 'Country_code'
			}
		})
	}
];

export default testSteps;


